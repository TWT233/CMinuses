/**
 * lexical token types naming rule:
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

