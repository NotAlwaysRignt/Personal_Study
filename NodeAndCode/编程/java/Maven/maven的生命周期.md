软件的生产过程是一套完整的流程,不只是简单的写代码然后编译成功就行
完整的软件构建过程包括 清理(代码重写后要清理原来代码的编译文件)、 编译、 测试、打包、集成测试、验证、部署等步骤

maven 把上面的过程逻辑划分为3个生命周期:

clean生命周期(负责清理)
default生命周期(编译,测试,打包等)
site生命周期 (部署)

每个生命周期又被划分为很多阶段(phase),这些阶段即对应了我们 maven 在命令行中的具体命令
下面介绍下maven每个生命周期下的命令,并对最常用的命令(比如 "mvn compile")做介绍:

clean生命周期:

    pre-clean    ：执行清理前的工作；
    clean    ：清理上一次构建生成的所有文件；(即删除target目录)
    post-clean    ：执行清理后的工作


default生命周期(最核心):

    validate
    initialize
    generate-sources
    process-sources
    generate-resources
    process-resources    ：复制和处理资源文件到target目录，准备打包；
    compile    ：编译项目的源代码；
    process-classes
    generate-test-sources
    process-test-sources
    generate-test-resources
    process-test-resources
    test-compile    ：编译测试源代码；
    process-test-classes
    test    ：运行测试代码；
    prepare-package
    package    ：打包成jar或者war或者其他格式的分发包；
    pre-integration-test
    integration-test
    post-integration-test
    verify
    install    ：将打好的包安装到本地仓库，供其他项目使用；
    deploy    ：将打好的包安装到远程仓库，供其他项目使用；


site生命周期

    pre-site
    site    ：生成项目的站点文档；
    post-site
    site-deploy    ：发布生成的站点文档



maven的每个生命周期相互独立,不需要顺序,也就是说,在执行default生命周期内的命令时,比如 mvn compile,我们可以不先执行 mvn clean

但是每个生命周期内部执行必须是顺序的,比如在 执行 mvn compile,那么mvn会自动地去执行 default生命周期中 compile前面的命令,比如 mvn process-resources (复制和处理资源文件到target目录，准备打包),再入运行package命令时,该生命周期中位于package命令前面的(compile,test等)会自动运行

maven是利用插件来执行生命周期下的每个阶段的,比如"mvn package"命令,maven要先去下载能够执行package的插件,才能执行这个命令.

对于每个插件,maven还进行进一步细分,仍以package为例,这个命令完成的是一个打包功能,对打包maven还细分了很多打包的类型,比如shade,source等类型,而对每个类型,maven由进行了细分,比如jar,jar-no-fork等,我们可以在pom.xml文件中对 要引入的插件以及这些插件的细节 进行配置说明.

当然我们也可以不对每个命令做具体配置说明,比如只是调用 maven package, 那maven会选择默认配置
