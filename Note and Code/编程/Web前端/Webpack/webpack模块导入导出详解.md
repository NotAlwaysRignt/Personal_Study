模块化编程中 `require/export`,`import/require`是很常见的,它们的用发多样,初学时我被搞得有点晕,这里要解释一下
### 注意 nodejs 和 webpack 打包的 js 中 js 的区别
webpack 是要 nodejs 运行环境支持的, `webpack.config.js` 就是一个 nodejs 环境下运行的文件
nodejs 和 webpack 打包的 js是不一样的,我们在`webpack.config.js`中看到了`modules.export`关键子,这里的 `export` 是 nodejs 的模块管理方法,和 webpack是有区别的
简单地说就是,nodejs 中的 js,并不支持 webpack 的这套 `require/export`,`import/require`规则,当然最新版本的nodejs 已经开始考虑这样做了.
我们要明确的就是,`webpack.config.js`是 nodejs 环境下运行的文件,而 webpack 要打包的 js 文件,它们的  `require/export`,`import/require` 在 nodejs 环境下运行不起来,因此别把 nodejs 的 export 和 webpack 的export 搞一块了
同样要知道的是 ,nodejs 中的 js 文件其 require,import 等函数必须由 nodejs 环境支持,在浏览器上运行不了
### import 和 require 的区别
直接上结论 import 和 require 在使用效果上是一样的,require 是 Commonjs社区中的开发者自发草拟的规则,然后逐步被大众使用
import 是 ES6 提出的标准,从某种角度上说,import 是官方的, require 是从民间自发形成的规范,目前各大厂商都逐步会遵从这个标准去支持import,因此可以放心地去用
参见文章:https://zhuanlan.zhihu.com/p/32112925
