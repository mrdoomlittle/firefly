# ifndef __ffly__phy__clock__h
# define __ffly__phy__clock__h
# include "../ffint.h"

// rename this
# define PHY_TIME_PERIOD 0.2
ff_u32_t extern phy_clock;

void ffly_phy_clock_tick(void);
# endif /*__ffly__phy__clock__h*/
