[TOC]
webpack 是当今前端构建工具中最常用的模块打包工具,帮助能够帮助开发者模块化开发,下面是其使用思路
### 区别 nodejs 和 webpack 打包的 js 
webpack 需要 nodejs 运行环境支持, `webpack.config.js` 就是一个 nodejs 环境下运行的 js 文件
要注意的是, 我们使用 webpack 开发时,最后要打包的 js,css 等文件最终是要跑在浏览器上的,这里被打包的 javascript 运行于 nodejs 下书写的 javascript 是不相同的,它们遵循不同的语法
比如,我们在`webpack.config.js`中看到了`modules.export`关键字, `export` 是 nodejs 的模块管理方法,像这种 nodejs 关键字,在浏览器是运行不起来的.
而我们在开发时用到`require/export`,`import/require`关键字,nodejs 并不支持,,当然最新版本的 nodejs 已经开始考虑这样做了.

总结就是,webpack 是需要nodejs 环境运行的软件,`webpack.config.js`是由 nodejs 编译解释的,需要遵循 nodejs 语法,而 webpack 要打包的 js 文件是遵循 ES6 语法(有的可能并不完全支持,但要明确的是和 nodejs 用的往往不是一套语法)而非 nodejs 语法的,这些js 文件由 webpack 对其进行编译解释

#### import 和 require 的区别
开发时中 `require/export`,`import/require`是很常见的,它们的用法多样,初步接触可能会有困惑
直接上结论: import 和 require 在使用效果上是一样的,require 是 Commonjs社区中的开发者自发草拟的规则,然后逐步被大众使用
import 是 ES6 提出的标准,从某种角度上说,import 是官方的, require 是从民间自发形成的规范,目前各大厂商都逐步会遵从这个标准去支持import,因此可以放心地去用
参见文章:https://zhuanlan.zhihu.com/p/32112925

### loader 和 plugin
loader 和 plugin 是`webpack.config.js`中最常涉及的两个概念
#### loader 和 plugin 的区别
webpack 本身只能打包遵循 commonjs 规范的文件,对于其它文件如 css,图片,vue文件等,是没有办法加载的,这就需要 loader 对其进行解析转化,使得 webpack 加载进来.
plugin 用于扩展 webpack 功能,当然 loader 也扩展了 webpack 功能,但 loader 只专注于文件的转换,plugin 不局限于此. plugin 作用于整个打包过程

