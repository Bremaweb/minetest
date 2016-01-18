#include <fcntl.h>
#include <sys/mman.h>

struct LinkedMem {
#ifdef WIN32
	UINT32 uiVersion;
	DWORD uiTick;
#else
	uint32_t uiVersion;
	uint32_t uiTick;
#endif
	float fAvatarPosition[3];
	float fAvatarFront[3];
	float fAvatarTop[3];
	wchar_t name[256];
	float fCameraPosition[3];
	float fCameraFront[3];
	float fCameraTop[3];
	wchar_t identity[256];
#ifdef WIN32
	UINT32 context_len;
#else
	uint32_t context_len;
#endif
	unsigned char context[256];
	wchar_t description[2048];
};

LinkedMem *lm = NULL;

void initMumble() {

#ifdef WIN32
	HANDLE hMapObject = OpenFileMappingW(FILE_MAP_ALL_ACCESS, FALSE, L"MumbleLink");
	if (hMapObject == NULL)
		return;

	lm = (LinkedMem *) MapViewOfFile(hMapObject, FILE_MAP_ALL_ACCESS, 0, 0, sizeof (LinkedMem));
	if (lm == NULL) {
		CloseHandle(hMapObject);
		hMapObject = NULL;
		return;
	}
#else
	char memname[256];
	snprintf(memname, 256, "/MumbleLink.%d", getuid());

	int shmfd = shm_open(memname, O_RDWR, S_IRUSR | S_IWUSR);

	if (shmfd < 0) {
		return;
	}

	lm = (LinkedMem *) (mmap(NULL, sizeof (struct LinkedMem), PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0));

	if (lm == (void *) (-1)) {
		lm = NULL;
		return;
	}
#endif
}