/*
******�����ַ���������******
���뻷�����и���Ŀ�������п�������Unicode�ַ����Ͷ��ֽ��ַ���
Unicode��ӦTCHAR
���ֽڶ�Ӧchar

���ں�����˵һ�㶼������W���Ƕ�ӦUnicode ����A���Ƕ�Ӧ���ֽ�

����SetWindowTextW ����������Ƕ�ӦUnicode ���������������Ҫ��TCHAR
(��ʵSetWindowTextAֻ�ǰѲ���ת�����ٵ���SetWindowTextW����)

������ʹ��MessageBox��GetWindowText��SetWindowTextʱ��������������⣬Ҫô�ڹ������Ըĳɶ��ֽ��ַ���
����Ҫ��charֱ�Ӿ��ô�A�ĺ��������Ҫ�ô�W�ĺ�������������ǰ����TEXT���Ա�����Ч������MessageBox(hwnd,szChar,TEXT("char"),0);
�������MessageBox�͸㶨������Ŀ����->�����У���Uicode�ĳɶ��ַ��Ρ�

******���������ѡ��******
char����ͬ�����Դ洢���֣���������Ȼ��̫��ȫ����������Ҫ�ú��ֻ�����Unicode�ã��ο�MessageBox��ЩĬ��
�ÿ��ֽڣ�����ֻҪ��TCHAR���������飬��д�ַ���ʱ��TEXT()�꼴�� getText��ЩҲ��Ĭ��ʹ��Unicode��ȡ
setText��MessageBoxҲ��Ĭ��Unicode����������ȡ��û����

******����Ҫ�޸ı༭��������Ĵ�С�ͷ��
SetTextColor���ڸı�������ɫ
��Ҫ�ı��С�����
					LOGFONT lf;//����ṹ�壬�ṹ��洢���������Ϣ
					memset(&lf, 0, sizeof(LOGFONT));   //�Ƚ��������Ϣ�ÿ�
					
					//void *memset(void *s, int ch, size_t n);
					//�������ͣ���s�е�ǰλ�ú����n���ֽ� ��typedef unsigned int size_t ���� ch �滻������ s 

					*lf.lfFaceName=WCHAR("����");//���������֣�ϵͳ���Զ���Ѱ���Ƿ��к͡����顱��ƥ�������
					lf.lfWidth=15; //�������ǿ��������Զ�������ĸ߶ȺͿ�ȵ�������Ϣ����Ȼ���Կ��ǽ��LOWORD(wParam);��HIWORD(wParam);
					lf.lfHeight=15;//����ȡĬ�ϵ������С��Ϣ��LOGFONT�ṹ���ڻ��������������Ա���Weight������Ҳ����ͨ���޸�lf��Ա��ֵ������


					HFONT m_hFont;
					m_hFont=CreateFontIndirect(&lf);//������������
					SendMessage(hwndEdit,WM_SETFONT,(WPARAM)m_hFont,0);//�ⲿ�Ϳ��԰������Զ�����������ø�����ָ���Ĵ�����
					//hwndEdit�����Ǵ������ڵľ��������ϸ�����ݿ���ѧϰCreateFontIndirect���÷�

******�����ַ����Ĵ���******
ͨ������swscanf��wsprintf��֧��%fת�壬�������ַ���ת�����ַ���ʱ��vs��swcanf�ſ������ƶ�ȡ��ȣ���ѡswscanf
��wsprintf��swcanf��֧��������ʽ�������Ҫ������(����������)д��wsprintf�Ļ���Ҫ��wsprintf�ˣ�����һ�㲻��ô����
���ǿ��԰Ѿ������ڴ����ַ�����(�ӻ�ȡ�Ĵ����ַ�����ɸѡ���ݿ����ø�Ч��C\C++)��Ȼ�����ǰ��ַ����ݽ���pythonת��
ת����������������Ч�����������python����C\C++
*/