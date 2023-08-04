#ifndef PACKAGE_HEADER_H
#define PACKAGE_HEADER_H

struct PackageHeader
{

	PackageHeader()
	{
		numChunks = 0;
		totalSize = 0;
	}

	PackageHeader(char* version, char* name)
	{

		if (version != nullptr)
			for (int i = 0; i < PACKAGE_NAME_SIZE; i++)
			{
				versionString[i] = version[i];
				if (version[i] == '\0')
				{
					break;
				}
				if (i == PACKAGE_NAME_SIZE - 1) { versionString[i] = '\0'; }
			}

		if (name != nullptr)
			for (int i = 0; i < PACKAGE_NAME_SIZE; i++)
			{
				packageName[i] = name[i];
				if (name[i] == '\0')
				{
					break;
				}
				if (i == PACKAGE_NAME_SIZE - 1) { packageName[i] = '\0'; }
			}

		numChunks = 0;
		totalSize = 0;

	}

	static const unsigned int PACKAGE_VERSION_SIZE = 20;
	static const unsigned int PACKAGE_NAME_SIZE = 20;


	char			packageName[PACKAGE_NAME_SIZE];
	char			versionString[PACKAGE_VERSION_SIZE];
	unsigned int	numChunks;
	unsigned int	totalSize;   // size of file (without package header)
};

#endif