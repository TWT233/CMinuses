/**
 * lexical token type naming rules:
 * 
 *      [`group`_[`category`_]]<name>
 * 
 * `group`:
 *      represents the functionality of tokens in this type.
 *      available options for this field:
 *          - DATA  :   raw data tokens
 *          - KWD   :   short for keyword
 *          - OP    :   short for operator
 * 
 * `category`:
 *      smaller divide for a group, usually tells specific usage of this token.
 * 
 * `name`:
 *      name of this type of tokens.
 *      easy to memory.
 *      easy to understand.
 * 
 * e.g.
 *      LP
 *      KWD_INT
 *      OP_REL_GE
 * 
 */

/**
 * lexical token type number assignment rules:
 * 
 * typeno is a 32-bit unsigned int, which be divided into parts here:
 * 
 *          0xFF|FF|FF|FF
 *            --|--|--|--
 *            P1|P2|P3|P4
 * 
 * P1: the code of group:
 *      - DATA  : 01
 *      - OP    : 02
 *      - KWD   : 03
 * 
 * P2: the code of category (optional):
 *      there are categories under OP now,
 *      for OP :
 *          - REL   : 01
 *          - BOO   : 02
 *          - CAL   : 03
 *     set this field to zero for no-cate token types.
 * 
 * P3 & P4: the code of name, P3 is necessary while
 *          P4 is enabled when this type of token is a ASCII char
 *          e.g. ">" "!" "=" "+"
 *
 */

#define G_DATA 01
#define G_OP 02
#define G_KWD 03

#define C_OP_REL 0101
#define C_OP_BOO 0102
#define C_OP_CAL 0103

#define gen_token_code(p, n, c, g) (0x##g##c##n##00 + p)

// #define DATA_INT gen_token_code(G_DATA, 00, 01, 00)
// #define DATA_FLOAT gen_token_code(G_DATA, 00, 02, 00)

//***********************************************

#define OP_REL_GE gen_token_code(00, 01, C_OP_REL)
#define OP_REL_LE gen_token_code(00, 02, C_OP_REL)
#define OP_REL_G gen_token_code('>', 03, C_OP_REL)
#define OP_REL_L gen_token_code('<', 04, C_OP_REL)
#define OP_REL_EQ gen_token_code(00, 05, C_OP_REL)
#define OP_REL_NE gen_token_code(00, 06, C_OP_REL)
//-----------------------------------------------
#define OP_BOO_AND gen_token_code(00, 01, C_OP_BOO)
#define OP_BOO_OR gen_token_code(00, 02, C_OP_BOO)
#define OP_BOO_NOT gen_token_code('!', 03, C_OP_BOO)
//-----------------------------------------------
#define OP_CAL_PLUS gen_token_code('+', 01, C_OP_CAL)
#define OP_CAL_MINUS gen_token_code('-', 02, C_OP_CAL)
#define OP_CAL_STAR gen_token_code('*', 03, C_OP_CAL)
#define OP_CAL_DIV gen_token_code('/', 04, C_OP_CAL)
//-----------------------------------------------
#define OP_ASSIGN gen_token_code('=', 01, 00)
#define OP_DOT gen_token_code('.', 02, 00)

//***********************************************

#define KWD_INT gen_token_code(00, 01, 00, G_KWD)
#define KWD_FLOAT gen_token_code(00, 02, 00, G_KWD)
#define KWD_STRUCT gen_token_code(00, 03, 00, G_KWD)
#define KWD_RETURN gen_token_code(00, 04, 00, G_KWD)
#define KWD_IF gen_token_code(00, 05, 00, G_KWD)
#define KWD_ELSE gen_token_code(00, 06, 00, G_KWD)
#define KWD_WHILE gen_token_code(00, 07, 00, G_KWD)

//***********************************************

#define ID gen_token_code(00, 01, 00, 00)
#define SEMI gen_token_code(';', 02, 00, 00)
#define COMMA gen_token_code(',', 03, 00, 00)
#define LP gen_token_code('(', 04, 00, 00)
#define RP gen_token_code(')', 05, 00, 00)
#define LB gen_token_code('[', 06, 00, 00)
#define RB gen_token_code(']', 07, 00, 00)
#define LC gen_token_code('{', 08, 00, 00)
#define RC gen_token_code('}', 09, 00, 00)
