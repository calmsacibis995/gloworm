#ifndef KERNEL_MACROS_H
#define KERNEL_MACROS_H

#define O_BITSMAJOR	7	/* # of SVR3 major device bits */
#define O_BITSMINOR	8	/* # of SVR3 minor device bits */
#define O_MAXMAJ	0x7f	/* SVR3 max major value */
#define O_MAXMIN	0xff	/* SVR3 max major value */

#define L_BITSMAJOR	14	/* # of SVR4 major device bits */
#define L_BITSMINOR	18	/* # of SVR4 minor device bits */
#define L_MAXMAJ	0xff	/* Although 14 bits are reserved, 
the 3b2 major number is restricted to 8 bits. s*/

#define L_MAXMIN	0x3ffff	/* MAX minor for 3b2 software drivers.
For 3b2 hardware devices the minor is restricted to 256 (0-255) */

/*
 * major part of a device external from the kernel (same as emajor below)
 */
#define	major(x)	(int)(((unsigned)x>>O_BITSMINOR)&O_MAXMAJ)
#define	bmajor(x)	(int)(((unsigned)x>>O_BITSMINOR)&O_MAXMAJ)

/*
 * minor part of a device external from the kernel (same as eminor below)
 */
#define	minor(x)	(int)(x&O_MAXMIN)

/*
 *   emajor() allows kernel/driver code to print external major numbers
 *   eminor() allows kernel/driver code to print external minor numbers
 */
#define emajor(x)   (int)((((unsigned long)(x)>>L_BITSMINOR) > L_MAXMAJ)
#define eminor(x)	(int)((x)&L_MAXMIN)

#endif /* KERNEL_MACROS_H */
