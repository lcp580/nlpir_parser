20140622

----安装说明：

将setup\PostgreSQL拷贝至pg的目录下覆盖即可。
默认安装目录是C:\Program Files\PostgreSQL\9.3，安装到其他目录会有问题。

然后安装中文分词的框架扩展
--创建中文分词解析器的扩展,nlpir_parser
create extension nlpir_parser;

--创建文本搜索器的配置
CREATE TEXT SEARCH CONFIGURATION nlpir_cfg (PARSER = nlpir_parser);

--必须执行下面很语句，以进行字符映射。否则执行to_tsvector时结果会为空，即没有结果

ALTER TEXT SEARCH CONFIGURATION nlpir_cfg ADD MAPPING FOR word WITH simple;

记得一定要安装segement分词函数啊

--segement
CREATE OR REPLACE FUNCTION  segement(TEXT) RETURNS TEXT
AS 'nlpir_parser.dll','ParagraphProcess'
LANGUAGE C STRICT;

----安装完成。

如果nlpir_parser.dll库的内部代码有变动，复制到相应目录后虽然一般说postgresql支持热部署，
但最好还是将postgresql服务重新启动一次为好。不然会出现莫名其妙的分词结果字符串为乱码的问题。
今天这个版本在pgadmin3下面没有问题.
但在命令行psql下面有时也就是说是偶尔(不是每次)还是有乱码。可能是windows命令行对utf8支持的问题。网上好像有过类似介绍。
这里用的是strcpy()。

建议，试试memcpy看看

这个版本直接调用segement分词函数得到分词结果字符串(以空格隔开每个词，类似英文句子)，然后原封不动地套用test_parser.c的样子，实现中文全文索引。
nlpir_parser_20140622.c


psql ok:
select to_tsvector('nlpir_cfg',segement('我是中国人民的好儿子'));

select to_tsvector('nlpir_cfg',segement('我是中国人，我来自湖北省江陵县'));

select to_tsvector('nlpir_cfg',segement('我是中国人'));

pgadmin3 ok:
select to_tsvector('nlpir_cfg',segement('我是中国人民的好儿子'));

select to_tsvector('nlpir_cfg',segement('我是中国人，我来自湖北省江陵县'));

select to_tsvector('nlpir_cfg',segement('我是中国人'));

--test ts_parser
psql ok:
select * from ts_parse('nlpir_parser',segement('我是中国人民的好儿子'));
select * from ts_parse('nlpir_parser',segement('我是中国人，我来自湖北省江陵县'));
select * from ts_parse('nlpir_parser',segement('我是中国人'));

pgadmin3 ok:
select * from ts_parse('nlpir_parser',segement('我是中国人民的好儿子'));
select * from ts_parse('nlpir_parser',segement('我是中国人，我来自湖北省江陵县'));
select * from ts_parse('nlpir_parser',segement('我是中国人'));

--englist and chinese test ,ok:
select * from ts_parse('nlpir_parser',segement('I am china,I love my home!我是中国人，我来自湖北省江陵县'));

select to_tsvector('nlpir_cfg',segement('I am china,I love my home!我是中国人，我来自湖北省江陵县'));


20140621
这个版本分词有时会出现乱码，特别是在PSQL命令行模式下，在pgadmin3客户端操作出现的概率较低。
一般指重复对同样一个句子分词
test_parser_vc20140621.c

--创建中文分词解析器的扩展,nlpir_parser
create extension nlpir_parser;

--修改已存在的文本搜索器的名字，由nlpir_cfg改名为nlpircfg,这里不用执行
ALTER TEXT SEARCH CONFIGURATION  nlpir_cfg  RENAME TO nlpircfg;

--创建文本搜索器的配置
CREATE TEXT SEARCH CONFIGURATION nlpir_cfg (PARSER = nlpir_parser);

--必须执行下面很语句，以进行字符映射。否则执行to_tsvector时结果会为空，即没有结果

ALTER TEXT SEARCH CONFIGURATION nlpir_cfg ADD MAPPING FOR word WITH simple;

--test 
--偶尔有时会出现乱码，大多数情况下却不会出现。
select * from ts_parse('nlpir_parser','我是中国人民的好儿子');

select * from ts_parse('nlpir_parser','我是中国人，我来自湖北省江陵县');

select to_tsvector('nlpir_cfg','我是中国人民的好儿子');


--以下在psql命令行下第一次正常，多次执行同样语句时会出现乱码，在pgadmin3环境下还好。没有乱码。是否内存问题?
select to_tsvector('nlpir_cfg','我是中国人，我来自湖北省江陵县');

select to_tsvector('english','I am a chinese of sun');


--segement
CREATE OR REPLACE FUNCTION  segement(TEXT) RETURNS TEXT
AS 'nlpir_parser.dll','ParagraphProcess'
LANGUAGE C STRICT;

select * from segement('我是中国人');
select * from segement('我是中国人，我来自湖北省江陵县');
select * from segement('我是中国人民的好儿子');

///

20140206

nlpir_tokenize.c
基于nlpir的postgresql 9.3的中文分词函数。

编译说明：
vc2010,设置为使用c方式编译,即/TC，得到nlpir_parser.dll库。

安装说明：
将nlpir_parser.dll拷贝到pg的安装目录的lib目录下，即C:\Program Files\PostgreSQL\9.3\lib
特别注意要将nlpir的NLPIR.dll拷贝到pg的bin目录下，即C:\Program Files\PostgreSQL\9.3\bin
不能拷贝到pg的安装目录的lib目录下，否则会出错。
将nlpir的data字典文件拷贝到pg的扩展目录下，即C:\Program Files\PostgreSQL\9.3\share\tsearch_data
这在程序中已经写好了，安装时不能改变。

然后进入psql或者pgAdmin III的控制台的某个数据库下面进行操作
CREATE OR REPLACE FUNCTION  TOKENIZE(TEXT) RETURNS TEXT
AS 'nlpir_parser.dll','tokenize'
LANGUAGE C STRICT;

OR:(如果此函数存在会提示要先删除此函数)
CREATE  FUNCTION  tokenize(text) RETURNS TEXT
AS 'nlpir_parser.dll','tokenize'
LANGUAGE C;

使用说明：
select tokenize('我是中国人');
结果：
mytestdb=# select tokenize('我是中国人');
注意:  Chinese Parser Lib NLPIR lcp580 ok init!""
          tokenize
-----------------------------
 我/rr 是/vshi 中国/ns 人/n
(1 行记录)

改进：
写一个sql语句，复制到extension目录下，然后简单执行以下语句即可完成安装：
CREATE EXTENSION TOKENIZE;
具体例子可以参考postgresql 9.3源代码目录contrib下的相关模块。
zhparser--1.0.sql
zhparser.control
zhparser--unpackaged--1.0.sql