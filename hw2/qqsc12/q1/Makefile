# 定義編譯器與參數
CXX = g++
CXXFLAGS = -O3 -std=c++11

# 預設目標
all: app

# 建立執行檔，並確保它依賴 main.cpp 和 Line.hpp
app: main.cpp Line.hpp
	$(CXX) $(CXXFLAGS) main.cpp -o app

# 執行程式
run: app
	./app

# 產生結果檔給老師比對
check: app
	./app > result.txt

# 清理暫存檔
clean:
	rm -f app result.txt