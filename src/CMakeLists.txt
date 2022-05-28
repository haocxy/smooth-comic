include_directories(.)

# 模块功能：和业务无关的很常用的逻辑，例如字符串处理等
# 特点：很小的静态库
# 注意：这个模块的二进制产物应该很小，最好多使用内联
add_subdirectory(core)

# 模块功能：主要的业务逻辑，项目要实现的具体功能都在这个模块中
# 特点：很大的静态库
add_subdirectory(body)

# 模块功能：没有任何逻辑，仅用于把 body 模块包装为动态库
add_subdirectory(body-dynamic)

# 模块功能：没有任何逻辑，仅用于把 body 模块包装为静态库
# 实际上，使用 body-static 和直接使用 body 是一样的，这样做是为了明确模块功能
add_subdirectory(body-static)

# 模块功能：
# 1：根据命令行参数决定是否为单例模式
# 2：若为单例模式则加载 body-dynamic 或 body-static
# 模块目的：
#  ：提升程序启动速度，当程序以单例模式运行时可以免去加载过多的库
# 特点：
#  ：较小的静态库
add_subdirectory(loader)

# 模块功能：提供程序入口，即提供可执行文件
add_subdirectory(entry)
