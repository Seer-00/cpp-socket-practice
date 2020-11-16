# cpp-socket-practice
## 构建信息
1. 语言：C++
2. GUI：Qt 5.9.0
3. 主要技术：Socket通信，多线程QThread etc.
4. exe文件生成方式：Visual Studio 2017, Release x64
## 使用方法
1. 保证Server开启，打开Client，左上角选择connect
2. Server向Client发送信息时，需选择Client的编号
## 主要功能
1. Server与Client是【一对多】的关系
2. Server与Client需同处一个局域网
3. Server可与Client进行双向通信
4. Server或Client离开通信，对方可被告知
5. 本机可同时充当Server和Client，此情况下，Client选择connect to localhost 或 输入IP:127.0.0.1
## 主要问题
1. 不涉及日志打印，数据备份等功能
2. 不考虑网络稳定性，不涉及断线重连
3. 不涉及手动调整端口，调整接收频率等
4. 不保证大文件的可靠传输
## 主要参数（不可更改）
1. Server Port：1234
2. Server 等待连接的方式：阻塞
3. Server 接收数据的方式：非阻塞
4. Server 接收数据的间隔：2s
5. Server 关闭信号："Server CLOSED."

5. Client 接收数据的方式：阻塞
6. Client / Server 接收/发送 缓冲区大小：8 x 1024 Bytes
7. Client 关闭信号："Client CLOSED."
