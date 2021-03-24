# MIPI-RFFE-Analyzer-for-Kingst
MIPI-RFFE-Analyzer-for-Kingst项目说明：
1. 这是基于金沙滩工作室开发的KingstVIS系列逻辑分析仪的一个协议解析插件；
2. 该插件用于解析MIPI-RFFE协议；
3. 一般来说，你需要有一台配套的逻辑分析仪即KingstVIS系列逻辑分析仪，该插件对你才有用；
4. 但即便没有，你仍然可以通过Demo模式看到该插件提供的测试序列来预览该插件是如何工作的。

使用方法：
1. 如果在Windows7以上版本的64bit操作系统，可以直接下载 MipiRffeAnalyzer.dll 文件并拷贝到Kingst VIS的Analyzer目录，一般在 "C:\Program Files\KingstVIS\Analyzer"；
2. 重启Kingst VIS程序，选择MIPI-RFFE解析器即可；
3. 如上述操作无法正常加载MIPI-RFFE，你可能需要安装VC2017运行库，下载地址为 https://aka.ms/vs/16/release/vc_redist.x64.exe
4. 其他操作系统请于 http://www.qdkingst.com/download/vis_sdk 下载SDK手册和示例，按手册第一章的方法编译该源码即可。
