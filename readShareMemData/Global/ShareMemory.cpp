/*!
 * --------------------------------------------------------------------------------------
 * @file: ShareMemory.c
 * --------------------------------------------------------------------------------------
 * @brief:
 * --------------------------------------------------------------------------------------
 * @author: meiyongpan
 * @version: V1.0
 * @date: 2016.2.18
 * --------------------------------------------------------------------------------------
 * @b:
 *    @li: 2016.2.18
 * --------------------------------------------------------------------------------------
*/

#include <sys/shm.h>

#include "ShareMemory.h"
#include "GlobalParameter.h"

ShareMemory::ShareMemory(QObject *parent) : QObject(parent)
{
    pBuffer = NULL;
    shareSize = 0;
}

ShareMemory::~ShareMemory()
{
    releaseShare();
}

/*!
 * --------------------------------------------------------------------------------------
 * @function name: closeShare
 * --------------------------------------------------------------------------------------
 * @brief:
 * --------------------------------------------------------------------------------------
 * @param: void
 * @return: bool
 * --------------------------------------------------------------------------------------
 * @author: meiyongpan
 * @date: 2016.2.18
 * --------------------------------------------------------------------------------------
 * @attention:
 * --------------------------------------------------------------------------------------
*/
bool ShareMemory::closeShare()
{
    //delete share
    if(shmctl(fd, IPC_RMID, 0) == -1)
    {
        PrintAlm("shmctl(IPC_RMID) failed");
        return false;
    }

   return true;
}

/*!
 * --------------------------------------------------------------------------------------
 * @function name: releaseShare
 * --------------------------------------------------------------------------------------
 * @brief:
 * --------------------------------------------------------------------------------------
 * @param: void
 * @return: bool
 * --------------------------------------------------------------------------------------
 * @author: meiyongpan
 * @date: 2016.2.18
 * --------------------------------------------------------------------------------------
 * @attention:
 * --------------------------------------------------------------------------------------
*/
bool ShareMemory::releaseShare()
{
    if(shmdt(pBuffer) == -1)
    {
        PrintAlm("shmdt failed\n");
        return false;
    }

    return true;
}

/*!
 * --------------------------------------------------------------------------------------
 * @function name: createShare
 * --------------------------------------------------------------------------------------
 * @brief:
 * --------------------------------------------------------------------------------------
 * @param: const char* key, int size, ShareMemory::AccessModeEnum mode
 * @return: bool
 * --------------------------------------------------------------------------------------
 * @author: meiyongpan
 * @date: 2016.2.18
 * --------------------------------------------------------------------------------------
 * @attention:
 * --------------------------------------------------------------------------------------
*/
bool ShareMemory::createShare(int key, int size, ShareMemory::AccessModeEnum mode)
{
    shareSize = size;
    accessMode = mode;

    switch(mode)
    {
        case ReadOnly:
        {
            fd = shmget(key, size, 0444);

            if(-1 == fd)
            {
                PrintAlm("Open file fail!\n");
                return false;
            }

            pBuffer = shmat(fd, (void*)0, 0);
            if(pBuffer == (void*)-1)
            {
                PrintAlm("shmat failed!\n");
                return false;
            }

            break;
        }
        default:
        {
            fd = shmget(key, size, 0666|IPC_CREAT);

            if(-1 == fd)
            {
                PrintAlm("Open file fail!\n");
                return false;
            }

            pBuffer = shmat(fd, (void*)0, 0);
            if(pBuffer == (void*)-1)
            {
                PrintAlm("shmat failed!\n");
                return false;
            }

            break;
        }

    }
    printf("shmid = %d",fd);
    return true;
}

