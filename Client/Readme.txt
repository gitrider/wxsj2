
//====== 1
编译：

添加环境变量：
BuildOut	// 输出目录
OutDir		// 生成目录

从16服务器上的 Program Share -> Share Source 中复制 EngineSDK 到自己本地， 和 Client 目录平级放置


最终的目录结构：

	存放项目的目录： Client	 	== 代码
			 Common  	== 代码
			 EngineSDK	== 引擎头文件和库文件
			 Bin		== 需要手动创建，内容（资源）从 VSS 获取， 
					   包括客户端、服务器的资源（还有两者都用到的资源）及可执行程序	
					   

代码编译后会自动拷贝到 Bin -> Client -> Game 下面， 所以要求目录一定要按上面的存放结构		 	


//====== 2
调试运行：

设置game项目的 属性 -》 调试 -》 工作目录 ：　指定到Bin -> Client 端资源的　Game　目录
设置 Game 目录下的System.cfg 里的服务器地址



//====== 3