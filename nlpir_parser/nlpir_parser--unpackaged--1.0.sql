-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION nlpir_parser" to load this file. \quit

ALTER EXTENSION zhparser ADD function nlpirprs_start(internal,integer);
ALTER EXTENSION zhparser ADD function nlpirprs_getlexeme(internal,internal,internal);
ALTER EXTENSION zhparser ADD function nlpirprs_end(internal);
ALTER EXTENSION zhparser ADD function nlpirprs_lextype(internal);
ALTER EXTENSION zhparser ADD text search parser nlpir_parsers;
