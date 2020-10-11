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
 *      - KWD   : 02
 *      - OP    : 03
 * 
 * P2: the code of category (optional):
 *      there are categories under OP now,
 *      for OP :
 *          - REL   : 01
 *          - CAL   : 02
 *          - BOO   : 03
 *     set this field to zero for no-cate token types.
 * 
 * P3 & P4: the code of name, P3 is necessary while
 *          P4 is enabled when this type of token is a ASCII char
 *          e.g. ">" "!" "=" "+"
 *          when P4 is enabled, set P3 == P4, otherwise P4 = 0
 *
 */
