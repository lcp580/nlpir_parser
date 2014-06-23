CREATE FUNCTION nlpirprs_start(internal, int4)
RETURNS internal
AS 'MODULE_PATHNAME'
LANGUAGE C STRICT;

CREATE FUNCTION nlpirprs_getlexeme(internal, internal, internal)
RETURNS internal
AS 'MODULE_PATHNAME'
LANGUAGE C STRICT;

CREATE FUNCTION nlpirprs_end(internal)
RETURNS void
AS 'MODULE_PATHNAME'
LANGUAGE C STRICT;

CREATE FUNCTION nlpirprs_lextype(internal)
RETURNS internal
AS 'MODULE_PATHNAME'
LANGUAGE C STRICT;

CREATE TEXT SEARCH PARSER nlpir_parser (
    START    = nlpirprs_start,
    GETTOKEN = nlpirprs_getlexeme,
    END      = nlpirprs_end,
    HEADLINE = pg_catalog.prsd_headline,
    LEXTYPES = nlpirprs_lextype
);
