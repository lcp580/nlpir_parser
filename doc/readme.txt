20140622

----��װ˵����

��setup\PostgreSQL������pg��Ŀ¼�¸��Ǽ��ɡ�
Ĭ�ϰ�װĿ¼��C:\Program Files\PostgreSQL\9.3����װ������Ŀ¼�������⡣

Ȼ��װ���ķִʵĿ����չ
--�������ķִʽ���������չ,nlpir_parser
create extension nlpir_parser;

--�����ı�������������
CREATE TEXT SEARCH CONFIGURATION nlpir_cfg (PARSER = nlpir_parser);

--����ִ���������䣬�Խ����ַ�ӳ�䡣����ִ��to_tsvectorʱ�����Ϊ�գ���û�н��

ALTER TEXT SEARCH CONFIGURATION nlpir_cfg ADD MAPPING FOR word WITH simple;

�ǵ�һ��Ҫ��װsegement�ִʺ�����

--segement
CREATE OR REPLACE FUNCTION  segement(TEXT) RETURNS TEXT
AS 'nlpir_parser.dll','ParagraphProcess'
LANGUAGE C STRICT;

----��װ��ɡ�

���nlpir_parser.dll����ڲ������б䶯�����Ƶ���ӦĿ¼����Ȼһ��˵postgresql֧���Ȳ���
����û��ǽ�postgresql������������һ��Ϊ�á���Ȼ�����Ī������ķִʽ���ַ���Ϊ��������⡣
��������汾��pgadmin3����û������.
����������psql������ʱҲ����˵��ż��(����ÿ��)���������롣������windows�����ж�utf8֧�ֵ����⡣���Ϻ����й����ƽ��ܡ�
�����õ���strcpy()��

���飬����memcpy����

����汾ֱ�ӵ���segement�ִʺ����õ��ִʽ���ַ���(�Կո����ÿ���ʣ�����Ӣ�ľ���)��Ȼ��ԭ�ⲻ��������test_parser.c�����ӣ�ʵ������ȫ��������
nlpir_parser_20140622.c


psql ok:
select to_tsvector('nlpir_cfg',segement('�����й�����ĺö���'));

select to_tsvector('nlpir_cfg',segement('�����й��ˣ������Ժ���ʡ������'));

select to_tsvector('nlpir_cfg',segement('�����й���'));

pgadmin3 ok:
select to_tsvector('nlpir_cfg',segement('�����й�����ĺö���'));

select to_tsvector('nlpir_cfg',segement('�����й��ˣ������Ժ���ʡ������'));

select to_tsvector('nlpir_cfg',segement('�����й���'));

--test ts_parser
psql ok:
select * from ts_parse('nlpir_parser',segement('�����й�����ĺö���'));
select * from ts_parse('nlpir_parser',segement('�����й��ˣ������Ժ���ʡ������'));
select * from ts_parse('nlpir_parser',segement('�����й���'));

pgadmin3 ok:
select * from ts_parse('nlpir_parser',segement('�����й�����ĺö���'));
select * from ts_parse('nlpir_parser',segement('�����й��ˣ������Ժ���ʡ������'));
select * from ts_parse('nlpir_parser',segement('�����й���'));

--englist and chinese test ,ok:
select * from ts_parse('nlpir_parser',segement('I am china,I love my home!�����й��ˣ������Ժ���ʡ������'));

select to_tsvector('nlpir_cfg',segement('I am china,I love my home!�����й��ˣ������Ժ���ʡ������'));


20140621
����汾�ִ���ʱ��������룬�ر�����PSQL������ģʽ�£���pgadmin3�ͻ��˲������ֵĸ��ʽϵ͡�
һ��ָ�ظ���ͬ��һ�����ӷִ�
test_parser_vc20140621.c

--�������ķִʽ���������չ,nlpir_parser
create extension nlpir_parser;

--�޸��Ѵ��ڵ��ı������������֣���nlpir_cfg����Ϊnlpircfg,���ﲻ��ִ��
ALTER TEXT SEARCH CONFIGURATION  nlpir_cfg  RENAME TO nlpircfg;

--�����ı�������������
CREATE TEXT SEARCH CONFIGURATION nlpir_cfg (PARSER = nlpir_parser);

--����ִ���������䣬�Խ����ַ�ӳ�䡣����ִ��to_tsvectorʱ�����Ϊ�գ���û�н��

ALTER TEXT SEARCH CONFIGURATION nlpir_cfg ADD MAPPING FOR word WITH simple;

--test 
--ż����ʱ��������룬����������ȴ������֡�
select * from ts_parse('nlpir_parser','�����й�����ĺö���');

select * from ts_parse('nlpir_parser','�����й��ˣ������Ժ���ʡ������');

select to_tsvector('nlpir_cfg','�����й�����ĺö���');


--������psql�������µ�һ�����������ִ��ͬ�����ʱ��������룬��pgadmin3�����»��á�û�����롣�Ƿ��ڴ�����?
select to_tsvector('nlpir_cfg','�����й��ˣ������Ժ���ʡ������');

select to_tsvector('english','I am a chinese of sun');


--segement
CREATE OR REPLACE FUNCTION  segement(TEXT) RETURNS TEXT
AS 'nlpir_parser.dll','ParagraphProcess'
LANGUAGE C STRICT;

select * from segement('�����й���');
select * from segement('�����й��ˣ������Ժ���ʡ������');
select * from segement('�����й�����ĺö���');

///

20140206

nlpir_tokenize.c
����nlpir��postgresql 9.3�����ķִʺ�����

����˵����
vc2010,����Ϊʹ��c��ʽ����,��/TC���õ�nlpir_parser.dll�⡣

��װ˵����
��nlpir_parser.dll������pg�İ�װĿ¼��libĿ¼�£���C:\Program Files\PostgreSQL\9.3\lib
�ر�ע��Ҫ��nlpir��NLPIR.dll������pg��binĿ¼�£���C:\Program Files\PostgreSQL\9.3\bin
���ܿ�����pg�İ�װĿ¼��libĿ¼�£���������
��nlpir��data�ֵ��ļ�������pg����չĿ¼�£���C:\Program Files\PostgreSQL\9.3\share\tsearch_data
���ڳ������Ѿ�д���ˣ���װʱ���ܸı䡣

Ȼ�����psql����pgAdmin III�Ŀ���̨��ĳ�����ݿ�������в���
CREATE OR REPLACE FUNCTION  TOKENIZE(TEXT) RETURNS TEXT
AS 'nlpir_parser.dll','tokenize'
LANGUAGE C STRICT;

OR:(����˺������ڻ���ʾҪ��ɾ���˺���)
CREATE  FUNCTION  tokenize(text) RETURNS TEXT
AS 'nlpir_parser.dll','tokenize'
LANGUAGE C;

ʹ��˵����
select tokenize('�����й���');
�����
mytestdb=# select tokenize('�����й���');
ע��:  Chinese Parser Lib NLPIR lcp580 ok init!""
          tokenize
-----------------------------
 ��/rr ��/vshi �й�/ns ��/n
(1 �м�¼)

�Ľ���
дһ��sql��䣬���Ƶ�extensionĿ¼�£�Ȼ���ִ��������伴����ɰ�װ��
CREATE EXTENSION TOKENIZE;
�������ӿ��Բο�postgresql 9.3Դ����Ŀ¼contrib�µ����ģ�顣
zhparser--1.0.sql
zhparser.control
zhparser--unpackaged--1.0.sql