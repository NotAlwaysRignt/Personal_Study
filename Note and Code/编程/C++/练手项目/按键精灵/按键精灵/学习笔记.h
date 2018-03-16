/*
******关于字符类型问题******
编译环境中有个项目属性其中可以设置Unicode字符集和多字节字符集
Unicode对应TCHAR
多字节对应char

对于函数来说一般都有最后带W的是对应Unicode 最后带A的是对应多字节

比如SetWindowTextW 这个函数就是对应Unicode 他的输入参数就需要是TCHAR
(其实SetWindowTextA只是把参数转换后再调用SetWindowTextW而已)

我们在使用MessageBox和GetWindowText、SetWindowText时都会遇到这个问题，要么在工程属性改成多字节字符集
或者要用char直接就用带A的函数，如果要用带W的函数，就在内容前加上TEXT（对变量无效），如MessageBox(hwnd,szChar,TEXT("char"),0);
如果想用MessageBox就搞定，在项目属性->常规中，把Uicode改成多字符段。

******关于字体的选择******
char类型同样可以存储汉字，但这样显然不太安全，所以我们要用汉字还是用Unicode好，何况MessageBox这些默认
用宽字节，我们只要用TCHAR来定义数组，在写字符串时用TEXT()宏即可 getText这些也会默认使用Unicode获取
setText，MessageBox也会默认Unicode输出，输出获取都没问题

******关于要修改编辑框内字体的大小和风格
SetTextColor用于改变字体颜色
若要改变大小及风格：
					LOGFONT lf;//定义结构体，结构体存储了字体的信息
					memset(&lf, 0, sizeof(LOGFONT));   //先将字体的信息置空
					
					//void *memset(void *s, int ch, size_t n);
					//函数解释：将s中当前位置后面的n个字节 （typedef unsigned int size_t ）用 ch 替换并返回 s 

					*lf.lfFaceName=WCHAR("楷书");//楷书是名字，系统会自动地寻找是否有和“楷书”相匹配的字体
					lf.lfWidth=15; //这里我们可以设置自定义字体的高度和宽度等其它信息，当然可以考虑结合LOWORD(wParam);和HIWORD(wParam);
					lf.lfHeight=15;//来获取默认的字体大小信息，LOGFONT结构体内还有其它字体属性比如Weight等我们也可以通过修改lf成员的值来配置


					HFONT m_hFont;
					m_hFont=CreateFontIndirect(&lf);//生成这种字体
					SendMessage(hwndEdit,WM_SETFONT,(WPARAM)m_hFont,0);//这部就可以把我们自定义的字体配置给我们指定的窗口了
					//hwndEdit是我们创建窗口的句柄，更详细地内容可以学习CreateFontIndirect的用法

******关于字符串的处理******
通过检测得swscanf和wsprintf不支持%f转义，但当从字符串转换到字符串时，vs中swcanf才可以限制读取宽度，首选swscanf
且wsprintf和swcanf都支持正则表达式，但如果要将整数(浮点数不行)写到wsprintf的话就要用wsprintf了，但是一般不这么处理
我们可以把精力放在处理字符串上(从获取的大量字符串中筛选数据可以用高效的C\C++)，然后我们把字符数据交给python转义
转义后的数据如果还想高效计算可以再让python交给C\C++
*/