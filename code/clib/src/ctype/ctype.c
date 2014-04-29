#include <ctype.h>

char _ctype[] = {0,	/* EOF */

	_IS_CTL, _IS_CTL, _IS_CTL, _IS_CTL, _IS_CTL, _IS_CTL, _IS_CTL,_IS_CTL,
	_IS_CTL, _IS_CTL|_IS_TAB,
                      _IS_CTL |_IS_TAB,
                               _IS_CTL |_IS_TAB,
                                        _IS_CTL |_IS_TAB,
                                                 _IS_CTL |_IS_TAB,
                                                          _IS_CTL,_IS_CTL,
	_IS_CTL, _IS_CTL, _IS_CTL, _IS_CTL, _IS_CTL, _IS_CTL, _IS_CTL,_IS_CTL,
	_IS_CTL, _IS_CTL, _IS_CTL, _IS_CTL, _IS_CTL, _IS_CTL, _IS_CTL,_IS_CTL,

	_IS_SP,  _IS_PUN, _IS_PUN, _IS_PUN, _IS_PUN, _IS_PUN, _IS_PUN,_IS_PUN,
	_IS_PUN, _IS_PUN, _IS_PUN, _IS_PUN, _IS_PUN, _IS_PUN, _IS_PUN,_IS_PUN,
	_IS_DIG, _IS_DIG, _IS_DIG, _IS_DIG, _IS_DIG, _IS_DIG, _IS_DIG,_IS_DIG,
	_IS_DIG, _IS_DIG, _IS_PUN, _IS_PUN, _IS_PUN, _IS_PUN, _IS_PUN,_IS_PUN,

	_IS_PUN, _IS_UPP|_IS_HEX,
    				  _IS_UPP|_IS_HEX,
							   _IS_UPP|_IS_HEX,
										_IS_UPP|_IS_HEX,
												 _IS_UPP|_IS_HEX,
														  _IS_UPP|_IS_HEX,
																  _IS_UPP,
	_IS_UPP, _IS_UPP, _IS_UPP, _IS_UPP, _IS_UPP, _IS_UPP, _IS_UPP,_IS_UPP,
	_IS_UPP, _IS_UPP, _IS_UPP, _IS_UPP, _IS_UPP, _IS_UPP, _IS_UPP,_IS_UPP,
	_IS_UPP, _IS_UPP, _IS_UPP, _IS_PUN, _IS_PUN, _IS_PUN, _IS_PUN,_IS_PUN,

	_IS_PUN, _IS_LOW|_IS_HEX,
					  _IS_LOW|_IS_HEX,
							   _IS_LOW|_IS_HEX,
										_IS_LOW|_IS_HEX,
												 _IS_LOW|_IS_HEX,
														  _IS_LOW|_IS_HEX, 
																  _IS_LOW,
	_IS_LOW, _IS_LOW, _IS_LOW, _IS_LOW, _IS_LOW, _IS_LOW, _IS_LOW,_IS_LOW,
	_IS_LOW, _IS_LOW, _IS_LOW, _IS_LOW, _IS_LOW, _IS_LOW, _IS_LOW,_IS_LOW,
	_IS_LOW, _IS_LOW, _IS_LOW, _IS_PUN, _IS_PUN, _IS_PUN, _IS_PUN,_IS_CTL
};
