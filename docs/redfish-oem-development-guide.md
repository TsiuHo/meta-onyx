# OpenBMC Redfish OEM 端點開發範本與參考指南

本文件提供完整的 bmcweb Redfish OEM 端點開發範本，涵蓋 **GET (讀取)**、**PATCH (更新)**、**POST (建立)** 三種 HTTP 方法的實作模式，可作為 ONYX 開發團隊新增 OEM 功能的參考範本。

---

## 1. 架構概覽

```text
┌─────────────────────────────────────────────────────────────────┐
│                    Redfish Client (curl / Web UI)               │
│         GET/PATCH/POST https://BMC_IP/redfish/v1/Oem/Onyx/...  │
└────────────────────────────┬────────────────────────────────────┘
                             │  HTTPS (port 443)
                             ▼
┌─────────────────────────────────────────────────────────────────┐
│                       bmcweb (C++ Web Server)                   │
│   ┌───────────────────────────────────────────────────────────┐ │
│   │  BMCWEB_ROUTE → Route Handler (oem_onyx_info.hpp)        │ │
│   │  1. 驗證權限 (privileges)                                 │ │
│   │  2. 解析 JSON body (PATCH/POST)                          │ │
│   │  3. 呼叫 D-Bus async API                                 │ │
│   └───────────────────────┬───────────────────────────────────┘ │
└───────────────────────────┼─────────────────────────────────────┘
                            │  D-Bus (system bus)
                            ▼
┌─────────────────────────────────────────────────────────────────┐
│                  D-Bus Service (onyx-service)                   │
│   Bus Name:   com.onyx.OemInfo                                  │
│   Object:     /com/onyx/oem_info                                │
│   Interface:  com.onyx.OemInfo                                  │
│   Properties: Version, BoardName, BiosVersion, Status           │
└─────────────────────────────────────────────────────────────────┘
```

---

## 2. 範本 A：GET 端點（讀取 D-Bus 屬性）

這是最基本的模式：從 D-Bus 讀取屬性，轉換為 Redfish JSON 回應。

### 2.1 bmcweb Route Handler

```cpp
// 檔案: redfish-core/lib/oem_onyx_info.hpp
#pragma once

#include "app.hpp"
#include "async_resp.hpp"
#include "query.hpp"
#include "registries/privilege_registry.hpp"
#include "utils/dbus_utils.hpp"

#include <sdbusplus/asio/property.hpp>
#include <memory>
#include <string>

namespace redfish
{

inline void requestRoutesOnyxOemInfo(App& app)
{
    BMCWEB_ROUTE(app, "/redfish/v1/Oem/Onyx/Info/")
        .privileges(redfish::privileges::getManager)
        .methods(boost::beast::http::verb::get)(
            [&app](const crow::Request& req,
                   const std::shared_ptr<bmcweb::AsyncResp>& asyncResp) {
                if (!redfish::setUpRedfishRoute(app, req, asyncResp))
                {
                    return;
                }

                // 設定 OData 基本欄位（必要）
                asyncResp->res.jsonValue["@odata.type"] =
                    "#OnyxOemInfo.v1_0_0.OnyxOemInfo";
                asyncResp->res.jsonValue["@odata.id"] =
                    "/redfish/v1/Oem/Onyx/Info";
                asyncResp->res.jsonValue["Id"] = "OnyxOemInfo";
                asyncResp->res.jsonValue["Name"] = "ONYX OEM Information";

                // 非同步查詢 D-Bus 所有屬性
                sdbusplus::asio::getAllProperties(
                    *crow::connections::systemBus,
                    "com.onyx.OemInfo",           // D-Bus 服務名稱
                    "/com/onyx/oem_info",         // D-Bus 物件路徑
                    "com.onyx.OemInfo",           // D-Bus 介面名稱
                    [asyncResp](
                        const boost::system::error_code& ec,
                        const dbus::utility::DBusPropertiesMap& props) {
                        if (ec)
                        {
                            BMCWEB_LOG_ERROR(
                                "D-Bus response error: {}", ec.value());
                            messages::internalError(asyncResp->res);
                            return;
                        }

                        const std::string* version = nullptr;
                        const std::string* boardName = nullptr;
                        const std::string* biosVersion = nullptr;
                        const std::string* status = nullptr;

                        const bool success =
                            sdbusplus::unpackPropertiesNoThrow(
                                dbus_utils::UnpackErrorPrinter(), props,
                                "Version", version,
                                "BoardName", boardName,
                                "BiosVersion", biosVersion,
                                "Status", status);

                        if (!success)
                        {
                            messages::internalError(asyncResp->res);
                            return;
                        }

                        if (version != nullptr)
                            asyncResp->res.jsonValue["Version"] = *version;
                        if (boardName != nullptr)
                            asyncResp->res.jsonValue["BoardName"] = *boardName;
                        if (biosVersion != nullptr)
                            asyncResp->res.jsonValue["BiosVersion"] =
                                *biosVersion;
                        if (status != nullptr)
                            asyncResp->res.jsonValue["Status"] = *status;
                    });
            });
}

} // namespace redfish
```

### 2.2 預期回應

```json
{
    "@odata.id": "/redfish/v1/Oem/Onyx/Info",
    "@odata.type": "#OnyxOemInfo.v1_0_0.OnyxOemInfo",
    "Id": "OnyxOemInfo",
    "Name": "ONYX OEM Information",
    "Version": "1.0.1",
    "BoardName": "ONYX-AST2600-EVB",
    "BiosVersion": "N/A",
    "Status": "OK"
}
```

---

## 3. 範本 B：PATCH 端點（更新 D-Bus 屬性）

適用於需要讓使用者透過 Redfish 修改設定的情境。

### 3.1 D-Bus 服務端（支援可寫入屬性）

```cpp
// 在 onyx-service/src/main.cpp 中，將 register_property 改為可寫入模式：
iface->register_property(
    "BoardName",
    std::string("ONYX-AST2600-EVB"),
    // Setter: 驗證並設定新值
    [](const std::string& newValue, std::string& oldValue) -> bool {
        if (newValue.empty())
        {
            return false;  // 拒絕空字串
        }
        oldValue = newValue;
        return true;
    },
    // Getter: 直接回傳目前值
    [](const std::string& value) -> std::string {
        return value;
    });
```

### 3.2 bmcweb PATCH Handler

```cpp
inline void requestRoutesOnyxOemInfoPatch(App& app)
{
    BMCWEB_ROUTE(app, "/redfish/v1/Oem/Onyx/Info/")
        .privileges(redfish::privileges::patchManager)
        .methods(boost::beast::http::verb::patch)(
            [&app](const crow::Request& req,
                   const std::shared_ptr<bmcweb::AsyncResp>& asyncResp) {
                if (!redfish::setUpRedfishRoute(app, req, asyncResp))
                {
                    return;
                }

                // 解析 JSON body，提取可選欄位
                std::optional<std::string> boardName;
                std::optional<std::string> biosVersion;

                if (!redfish::json_util::readJsonPatch(
                        req, asyncResp->res,
                        "BoardName", boardName,
                        "BiosVersion", biosVersion))
                {
                    return;  // 解析失敗，自動回傳 400 Bad Request
                }

                // 更新 BoardName（如果有提供）
                if (boardName)
                {
                    sdbusplus::asio::setProperty(
                        *crow::connections::systemBus,
                        "com.onyx.OemInfo",
                        "/com/onyx/oem_info",
                        "com.onyx.OemInfo",
                        "BoardName",
                        *boardName,
                        [asyncResp](const boost::system::error_code& ec) {
                            if (ec)
                            {
                                BMCWEB_LOG_ERROR(
                                    "Failed to set BoardName: {}", ec.value());
                                messages::internalError(asyncResp->res);
                                return;
                            }
                            messages::success(asyncResp->res);
                        });
                }

                // 更新 BiosVersion（如果有提供）
                if (biosVersion)
                {
                    sdbusplus::asio::setProperty(
                        *crow::connections::systemBus,
                        "com.onyx.OemInfo",
                        "/com/onyx/oem_info",
                        "com.onyx.OemInfo",
                        "BiosVersion",
                        *biosVersion,
                        [asyncResp](const boost::system::error_code& ec) {
                            if (ec)
                            {
                                messages::internalError(asyncResp->res);
                                return;
                            }
                        });
                }
            });
}
```

### 3.3 測試指令

```bash
# PATCH 更新 BoardName
curl -k -u root:0penBmc -X PATCH \
  -H "Content-Type: application/json" \
  -d '{"BoardName": "ONYX-MD116M-AST2600"}' \
  https://<BMC_IP>/redfish/v1/Oem/Onyx/Info
```

---

## 4. 範本 C：POST 端點（觸發動作）

適用於需要觸發某個操作的情境（例：執行診斷、重置計數器）。

### 4.1 D-Bus 服務端（註冊 Method）

```cpp
// 在 onyx-service/src/main.cpp 中新增 D-Bus method：
iface->register_method(
    "RunDiagnostics",
    [](const std::string& testType) -> std::tuple<bool, std::string> {
        // 執行診斷邏輯
        if (testType == "quick")
        {
            return {true, "Quick diagnostics passed"};
        }
        else if (testType == "full")
        {
            return {true, "Full diagnostics completed"};
        }
        return {false, "Unknown test type"};
    });
```

### 4.2 bmcweb POST Handler

```cpp
inline void requestRoutesOnyxDiagnostics(App& app)
{
    BMCWEB_ROUTE(app, "/redfish/v1/Oem/Onyx/Actions/RunDiagnostics/")
        .privileges(redfish::privileges::postManager)
        .methods(boost::beast::http::verb::post)(
            [&app](const crow::Request& req,
                   const std::shared_ptr<bmcweb::AsyncResp>& asyncResp) {
                if (!redfish::setUpRedfishRoute(app, req, asyncResp))
                {
                    return;
                }

                std::optional<std::string> testType;
                if (!redfish::json_util::readJsonAction(
                        req, asyncResp->res,
                        "TestType", testType))
                {
                    return;
                }

                crow::connections::systemBus->async_method_call(
                    [asyncResp](
                        const boost::system::error_code& ec,
                        const std::tuple<bool, std::string>& result) {
                        if (ec)
                        {
                            messages::internalError(asyncResp->res);
                            return;
                        }

                        auto [success, message] = result;
                        asyncResp->res.jsonValue["Success"] = success;
                        asyncResp->res.jsonValue["Message"] = message;
                    },
                    "com.onyx.OemInfo",       // D-Bus 服務
                    "/com/onyx/oem_info",     // D-Bus 物件
                    "com.onyx.OemInfo",       // D-Bus 介面
                    "RunDiagnostics",         // D-Bus method
                    testType.value_or("quick"));
            });
}
```

### 4.3 測試指令

```bash
# POST 觸發診斷
curl -k -u root:0penBmc -X POST \
  -H "Content-Type: application/json" \
  -d '{"TestType": "quick"}' \
  https://<BMC_IP>/redfish/v1/Oem/Onyx/Actions/RunDiagnostics
```

---

## 5. 業界廠商 OEM 實作參考

| 廠商 | OEM 端點位置 | 功能說明 |
| :--- | :--- | :--- |
| **NVIDIA** | `/redfish/v1/Systems/system/Oem/Nvidia` | GPU 遙測、NVLink 狀態、自訂功耗上限 |
| **Ampere** | `/redfish/v1/Chassis/chassis/Oem/Ampere` | CPU 核心狀態、RAS 錯誤計數器 |
| **IBM** | `/redfish/v1/Systems/system/LogServices/.../Oem/IBM` | PEL 事件日誌、韌體授權管理 |
| **Intel/OpenBMC** | `/redfish/v1/Managers/bmc/Oem/OpenBmc/Fan` | PID 溫控迴路參數、感測器覆寫 |
| **ONYX** | `/redfish/v1/Oem/Onyx/Info` | 板卡版本、BIOS 版本、運行狀態 |

---

## 6. 開發檢查清單

新增 OEM Redfish 端點時，請依循以下步驟：

- [ ] **1. D-Bus 服務設計**：確定 Service Name、Object Path、Interface、Properties/Methods
- [ ] **2. D-Bus daemon 實作**：在 `recipes-onyx/onyx-service/` 中修改或新增 daemon
- [ ] **3. bmcweb Route Handler**：建立 `redfish-core/lib/oem_onyx_<feature>.hpp`
- [ ] **4. Route 註冊**：在 `redfish-core/src/redfish.cpp` 中呼叫 `requestRoutes` 函式
- [ ] **5. Patch 生成**：使用 `git format-patch` 產生 bmcweb patch
- [ ] **6. BBAppend 更新**：在 `bmcweb_%.bbappend` 中新增 patch 引用
- [ ] **7. 測試驗證**：使用 `busctl` 和 `curl` 測試 D-Bus 與 Redfish 回應

---

## 7. 常見錯誤與排除

| 問題 | 原因 | 解決方式 |
| :--- | :--- | :--- |
| Redfish 回傳 500 Internal Error | D-Bus 服務未啟動 | `systemctl start com.onyx.OemInfo.service` |
| D-Bus property 取不到值 | unpack 型別不匹配 | 用 `busctl introspect` 確認 signature |
| bmcweb 編譯失敗 | patch 衝突 | 基於最新 bmcweb 重新 `git format-patch` |
| 路由無回應 | Route 未註冊 | 確認 `redfish.cpp` 中有呼叫 `requestRoutes` |

---

*文件更新時間：2026-08-12*
*適用平台：ONYX Healthcare AST2600 OpenBMC*
