#include "include/parser.h"
#include "include/types.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

parser_T* init_parser(lexer_T* lexer) 
{
	parser_T* parser = calloc(1, sizeof(struct PARSER_STRUCT));
	parser->lexer = lexer;
	parser->token = lexer_next_token(lexer);

	return parser;
}

token_T* parser_eat(parser_T* parser, int type)
{
	if (parser->token->type != type)
	{
		printf("Unexpected token %s found in parser. Was expecting %s.\n", token_to_str(parser->token), token_type_to_str(type));
		exit(1);
	}

	parser->token = lexer_next_token(parser->lexer);
	return parser->token;
}

AST_T* parser_parse(parser_T* parser) 
{
	/*
	switch (parser->token->type)
	{

	}
	*/
	return init_ast(AST_NOOP);
}

AST_T* parser_parse_id(parser_T* parser)
{
	char* value = calloc(strlen(parser->token->value) + 1, sizeof(char));
	strcpy(value, parser->token->value);
	parser_eat(parser, TOKEN_ID);

	if (parser->token->type == TOKEN_EQUALS) 
	{
		//parse assignment
		parser_eat(parser, TOKEN_EQUALS);
		AST_T* ast = init_ast(AST_ASSIGNMENT);
		ast->name = value;
		ast->value = parser_parse_expr(parser);
		return ast;
	}
		
	//parse variable
	AST_T* ast = init_ast(AST_VAR);
	ast->name = value;

	if(parser->token->type == TOKEN_TYPER) 
	{
		parser_eat(parser, TOKEN_TYPER);

		while(parser->token->type == TOKEN_ID)
		{
			ast->data_type = typename_to_int(parser->token->value);
			parser_eat(parser, TOKEN_ID);

			// datatype is of list type
			if(parser->token->type == TOKEN_OSQUAR) 
			{
				parser_eat(parser, TOKEN_OSQUAR);
				ast->data_type += typename_to_int(parser->token->value);
				parser_eat(parser, TOKEN_ID);
				parser_eat(parser, TOKEN_CSQUAR);
			}
		}
	}
	else 
	{
		if(parser->token->type == TOKEN_OPAREN)
		{
		/*
		if(parser->token->type == TOKEN_RETURN) 
		{
			parser_eat(parser, TOKEN_RETURN);
			*/
			ast->type = AST_CALL;
			ast->value = parser_parse_list(parser);
		}
	}

	return ast;
}


AST_T* parser_parse_block(parser_T* parser) 
{
	parser_eat(parser, TOKEN_OBRACE);
	AST_T* ast = init_ast(AST_COMP);

	while (parser->token->type != TOKEN_CBRACE)
	{
		list_push(ast->children, parser_parse_expr(parser));
	}

	parser_eat(parser, TOKEN_CBRACE);

	return ast;
}

AST_T* parser_parse_list(parser_T* parser)
{
	parser_eat(parser, TOKEN_OPAREN);
	AST_T* ast = init_ast(AST_COMP);
	list_push(ast->children, parser_parse_expr(parser));

	while (parser->token->type == TOKEN_COMMA)
	{
		parser_eat(parser, TOKEN_COMMA);
		list_push(ast->children, parser_parse_expr(parser));
	}

	parser_eat(parser, TOKEN_CPAREN);

	if(parser->token->type == TOKEN_TYPER) 
	{
		parser_eat(parser, TOKEN_TYPER);

		while(parser->token->type == TOKEN_ID)
		{
			ast->data_type = typename_to_int(parser->token->value);
			parser_eat(parser, TOKEN_ID);

			// datatype is of list type
			if(parser->token->type == TOKEN_OSQUAR) 
			{
				parser_eat(parser, TOKEN_OSQUAR);
				ast->data_type += typename_to_int(parser->token->value);
				parser_eat(parser, TOKEN_ID);
				parser_eat(parser, TOKEN_CSQUAR);
			}
		}
	}

	//is funtion / ifstatement / loop / etc.
	if (parser->token->type == TOKEN_OBRACE)
	{
		// parser_eat(parser, TOKEN_OBRACE);
		// handle all as function for now
		ast->type = AST_FUNC;
		ast->value = parser_parse_compound(parser);
		//ast->value = parser_parse_block(parser);
	}

	return ast;
}

AST_T* parser_parse_int(parser_T* parser)
{
	int int_value = atoi(parser->token->value);
	parser_eat(parser, TOKEN_INT);

	AST_T* ast = init_ast(AST_INT);
	ast->int_value = int_value;

	return ast;
}

AST_T* parser_parse_expr(parser_T* parser)
{
	switch (parser->token->type)
	{
		case TOKEN_ID: return parser_parse_id(parser);
		case TOKEN_OPAREN: return parser_parse_list(parser);
		case TOKEN_INT: return parser_parse_int(parser);

		default: {
			printf("Found unexpected token %s in parser\n", token_to_str(parser->token));
			exit(1);
		};
	}
}

AST_T* parser_parse_compound(parser_T* parser)
{
	unsigned int should_close = 0;
	if(parser->token->type == TOKEN_OBRACE) 
	{
		parser_eat(parser, TOKEN_OBRACE);
		should_close = 1;
	}

	AST_T* compound = init_ast(AST_COMP);

	while (parser->token->type != TOKEN_EOF && parser->token->type != TOKEN_CBRACE)
	{
		list_push(compound->children, parser_parse_expr(parser));

		if (parser->token->type == TOKEN_SEMI) 
		{
			parser_eat(parser, TOKEN_SEMI);
		}
	}

	if(should_close)
	{
		parser_eat(parser, TOKEN_CBRACE);
	}

	return compound;
}