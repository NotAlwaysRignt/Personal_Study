1.����������
2.�û�������,������������ӦServletʵ���Ƿ����,��������,װ��Servlet�ഴ��һ��Servletʵ��

3.����init(ServletConfig)����
4.����service���� (Servlet Request Servlet Response)

֮�������ͬ����������,����ӦServletʵ������������ֱ��ִ�� 3 �� 4

5.�������ر�ʱ,���� Servlet �� Destroy() ����,����ʵ��


����Servlet��(�����Զ����д��)����ʱ�ڵĲ���:

1.�����web.xmlû���ر�˵��,��ôServlet�ಢ������Tomcat����������ʱ�����ص�,�������û���һ�η���URLʱ,
���URL����Ӧ��Servlet��Ż�����乹�캯����ʵ����,Servlet�౻ʵ��ʱ���ȵ��ù��캯��,���Ż���� init()
����.һ����ʵ��������һֱ�������ڴ���,ֱ���������رղű�����.

2.��web.xml�е�<Servlet></Servlet>��ǩ�����<loadon-startup></loadon-startup>��ǩ,��ô���Servlet���
���ڷ�����������ʱ��ͱ����ض������û���һ�η���ʱʵ����,<loadon-startup></loadon-startup>��ǩ������ԽС
���ȼ�Խ��,Խ�ȿ�ʼ����,�������� <loadon-startup> 1 </loadon-startup> ����,
�ȱ����<loadon-startup> 2 </loadon-startup>��Ҫ�ȿ�ʼ����.

3.��Tomcat��������,��Դ���뱻�޸ĺ�,Tomcat���Զ����±������,������ʵ���� ֮ǰ�Ѿ�������Servletʵ��