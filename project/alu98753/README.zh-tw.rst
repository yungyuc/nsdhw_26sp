========================================================================
LockFreeReplay: 用於強化學習的零拷貝無鎖環形緩衝區
========================================================================

`[English Version] <README.rst>`__

基本資訊 (Basic Information)
============================

LockFreeReplay 是一個 C++ 函式庫，為強化學習 (RL) 中的經驗回放 (experience replay) 提供單一生產者/單一消費者 (SPSC) 的無鎖 (lock-free) 環形緩衝區，並搭配零拷貝 (zero-copy) 的 Python 介面。

程式碼將會託管在一個新的 GitHub 儲存庫： https://github.com/alu98753/lockfree-replay

在分散式或高吞吐量的 RL 資料管道中，我們需要記憶體緩衝區來儲存轉換資料（例如 state, action, reward 等）。現有的實作存在兩個主要瓶頸：

1. 在 Python 中儲存資料，或使用基於lock的 C++ 佇列，會導致全域直譯器鎖 (GIL) 爭用，在並發讀寫操作時造成執行緒停滯。
2. 在 C++（環境模擬器/回放緩衝區）與 Python（訓練迴圈）之間傳輸資料需要昂貴的記憶體拷貝，這會白白消耗 CPU 週期與記憶體頻寬。

預計解決的問題 (Problem to Solve)
=================================

LockFreeReplay 試圖解決的主要問題是「消除回放資料路徑上的鎖護爭用」，並實現在沒有記憶體拷貝的情況下於 C++ 和 Python (NumPy) 之間共享記憶體緩衝區。

儲存 RL 轉換資料的多維陣列需要極快的寫入速度（來自生產者 actor）以及極快的批次採樣速度（來自消費者 learner）。標準的 Python 多行程 (multiprocessing) 或是依賴鎖護的 C++ 佇列都會受到鎖定開銷 (locking overhead) 的影響。此外，將採樣到的 mini-batches 傳回 Python 通常會導致記憶體拷貝。

LockFreeReplay 專注於提供一個無鎖的 C++ 資料結構來解決寫入爭用，並透過 pybind11 的 buffer protocol 將底層連續記憶體直接暴露給 Python，以完全避免記憶體拷貝。

潛在使用者 (Prospective Users)
==============================

建構單機 (single-node) RL 管道的研究人員與工程師。此專案專為需要高吞吐量資料收集，但又不想承擔重型分散式系統（如 Reverb）或 Python 多行程帶來的複雜性與開銷的使用者設計。

系統架構 (System Architecture)
==============================

LockFreeReplay 將以 C++ 開發，使用對 head 及 tail 索引的原子操作 (atomic operations) 來實現無鎖的 SPSC 環形緩衝區。它會為固定形狀與型態的轉換資料管理一塊無型別 (untyped) 的記憶體緩衝區。

在 C++ 端，一個 ``ReplayBuffer`` 類別會配置一塊連續的記憶體區塊。對這塊記憶體，它不依賴互斥鎖 (mutex) 來管理來自單一生產者與單一消費者執行緒的並發讀寫操作。

在 Python 端，這個 C++ 緩衝區會透過 pybind11 進行包裝。它利用 ``pybind11::buffer_info``，將這塊記憶體區塊直接以 NumPy ``ndarray`` 的形式暴露為「視圖 (view)」。

使用者可以從 Python（或 C++）端推入過渡資料，並從 Python 端採樣資料批次。採樣函式將回傳直接指向 C++ 記憶體區塊的 NumPy 視圖，確保絕對的零拷貝。

注意事項 (Note)
---------------

* 關於「無鎖 (lock-free)」，此實作依賴 ``std::atomic`` 與記憶體排序 (memory ordering) 模型，特意避免使用 ``std::mutex``。
* 範圍約束為單一生產者、單一消費者 (SPSC) 且具備有界容量 (bounded queue) 的佇列，並採用覆寫最舊資料的策略 (overwrite-oldest)。初始版本暫不支援多生產者情境。
* Python 視圖的生命週期必須受到保護，不能在 C++ 緩衝區被釋放後存活。這部份將透過智慧指標 (如 ``std::shared_ptr``) 來管理。

API 描述 (API Description)
==========================

主要的 ``ReplayBuffer`` 類別將提供建構函式，接受緩衝區容量與資料佈局（例如狀態與動作的維度 shape）。

C++ 的方法將包含用於附加資料的 ``push()``，以及用於檢索採樣資料的 ``sample()``。

透過 pybind11 綁定出來的 Python 介面則提供：
* ``buffer.push_batch(ndarray)``: 接收包含轉換資料的 NumPy 陣列。
* ``buffer.sample_views(batch_size)``: 回傳一個包含採樣小批量的 NumPy 陣列字典 (dictionary of NumPy arrays)。這些陣列都是直接映射 C++ 記憶體的零拷貝視圖。

工程基礎建設 (Engineering Infrastructure)
=========================================

* **建置系統**: 使用 CMake 設定專案並建置 pybind11 模組。
* **測試**: 使用 Google Test (C++) 測試無鎖邏輯的正確性（例如確保高並發 push/sample 時資料不遺失）。結合 ThreadSanitizer (TSan) 來偵測與防範 data races。使用 ``pytest`` (Python) 斷言檢查零拷貝行為（驗證底層記憶體位址）與功能正確性。
* **版本控制**: 使用 Git 追蹤程式碼變更，並確保定期的提交。
* **持續整合**: 使用 GitHub Actions 在 Linux 環境上針對每個 commit 自動執行 C++ 與 Python 的測試套件。
* **效能分析**: 將使用標準的 C++ timing 函式庫追蹤系統吞吐量 (operations per second) 與延遲百分位數 (latency percentiles)，以量化證明無鎖設計對比基礎 mutex 鎖設計的效能優勢。

排程計畫 (Schedule)
===================

第 1 週 (03/16 - 03/22):
  定義資料轉換的結構佈局。以 C++ 實作一個基礎的基於 mutex 的環形緩衝區，並使用 Google Test 撰寫單執行緒的正確性測試。

第 2 週 (03/23 - 03/29):
  使用 ``std::atomic`` 重構 C++ 環形緩衝區，轉型為 SPSC 無鎖實作。導入快取存取對齊 ``alignas(64)`` 以防止偽共享 (false sharing)。

第 3 週 (03/30 - 04/05):
  撰寫多執行緒壓力測試 (1 producer, 1 consumer)。在 CMake 中配置 ThreadSanitizer (TSan) 並驗證完全沒有 data races。

第 4 週 (04/06 - 04/12):
  實作 pybind11 wrapper。向 Python 暴露 Buffer 的創建與基本的 push 方法。

第 5 週 (04/13 - 04/19):
  開發核心的零拷貝視圖機制。實作 ``sample_views()``，透過 ``pybind11::buffer_info`` 將資料化為 NumPy 陣列回傳。

第 6 週 (04/20 - 04/26):
  透過 ``std::shared_ptr`` 管理物件生命週期，確保即便在 C++ 端銷毀前，Python 視圖也能安全保持。撰寫 Python 整合測試 (``pytest``) 以斷言檢查記憶體的共享狀態。

第 7 週 (04/27 - 05/03):
  設定 GitHub Actions 以建立 CI 流程。撰寫 benchmark 腳本，比較無鎖實作與基礎 mutex 版本的吞吐量差異。

第 8 週 (05/04 - 05/10):
  確認並完善文件，整理與清理程式碼，準備期末的專題簡報。

參考資料 (References)
=====================

* cpprb (C++ Replay Buffer for RL): https://github.com/ymd-h/cpprb
* modmesh (SimpleArray reference): https://github.com/solvcon/modmesh
