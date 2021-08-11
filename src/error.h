#ifndef __cplusplus
#define decltype typeof
#include <stdbool.h>
#endif
#include <assert.h>

#define Result_EX( OK, ERR ) result_##OK##_##ERR##_t
#define Result( OK, ERR )    Result_EX( OK, ERR )
#define ResultDecl( OK, ERR )     \
	typedef struct {          \
		bool _temp;       \
		bool is_ok;       \
		union {           \
			OK  _ok;  \
			ERR _err; \
		};                \
	} Result( OK, ERR )

#define Ok( VAL, OK, ERR )       \
	( ( Result( OK, ERR ) ){ \
	    .is_ok = true,       \
	    ._ok   = VAL,        \
	} )
#define Err( VAL, OK, ERR )      \
	( ( Result( OK, ERR ) ){ \
	    .is_ok = false,      \
	    ._err  = VAL,        \
	} )

#define unwrap( v )     ( v.is_ok ? (void)0 : __builtin_abort(), v._ok )
#define unwrap_err( v ) ( !v.is_ok ? (void)0 : __builtin_abort(), v._err )

#define if_let_Ok( IDENT, VAL )                                       \
	if ( ( VAL._temp = VAL.is_ok ) )                              \
		for ( decltype( VAL._ok ) IDENT = VAL._ok; VAL._temp; \
		      VAL._temp                 = false )
#define if_let_Err( IDENT, VAL )                                      \
	if ( ( VAL._temp = !VAL.is_ok ) )                             \
		for ( decltype( VAL._ok ) IDENT = VAL._ok; VAL._temp; \
		      VAL._temp                 = false )
