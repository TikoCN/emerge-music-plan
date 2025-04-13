# musicPlayer
 这是一个基于qt6与ffmpeg的本地播放器，相较与传统的本地播放器不同，此播放器致力模仿applymusic,winplayer,以实现更好动态效果  
 支持绝大多数音频格式  
 同时能将 *.flac* *.mp3* *.alac* 等主流音乐格式相互转换

***

# 特点与特性
 支持逐字动感歌词  
 以.hlrc为后缀的歌词格式  
 基于C++与qml带来更好的性能、更少内存占用  
 能够在线获得音乐歌词、封面的信息  
 来源包括且不限于：网易云音乐、qq音乐、bing、百度  

***

## 关于 .hlrc 文件
 这是在.lrc格式三改造，一种逐块标记的歌词形式  
 大概格式如下  
 >\[0,1000\]\(0,100\)这是\(0,200\)一\(250,350\)行\(400,500\)歌词  
 其中"\[\]"中包含两串数字，","前部分为**文本行**开始时间，","后数字部分为**文本行**结束时间  
 其中"\(\)"中包含两串数字，","前部分为**文本块**开始时间，","后数字部分为**文本块**结束时间  

***

# 界面截图
 ![界面](/mkImage/1.png "界面")  
 ![界面](/mkImage/2.png "界面")  
 ![界面](/mkImage/3.png "界面")  
 ![界面](/mkImage/4.png "界面")  
 ![界面](/mkImage/5.png "界面")  

***

# 如何使用
 1.下载对应平台的压缩包，解压即可 (~~暂未发布，预计于4.10日发布正式版。win版本已于4.13日发布，linux和macos将于4.17日左右发布)  
 2.将本地音乐文件目录加入路径  
 3.播放音乐  

***

# 更新计划
 基于文本对齐自动标记.hlrc文件  
 更多动画  
 更多配置  
 服务端？  

***

# 依赖库，以及特别鸣谢
 [ffmpeg](https://ffmpeg.org/) 音频处理，转码解码等  
 [taglib](https://taglib.org/) 提前音频标签  
 [fftw3](https://fftw.org/) 音频处理   
 [vcpkg](https://github.com/microsoft/vcpkg) 进行包管理  
 [Qt6.8.2](https://www.qt.io/zh-cn/) 界面

*** 

## 关于更新
 如何您有任何的建议，请在issue中，我将会尽快回复您的想法，以便将其加入更新计划中  