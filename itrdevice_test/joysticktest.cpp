#include "joysticktest.h"
#include <stdio.h>

void JoyStickTest()
{
    itr_device::StdJoyStick Joyst;
    char file_name[] ={"/dev/input/js1"};
    if(Joyst.Open(file_name))
    {
        S32 axiscount=0;
        S32 buttoncount=0;
        F32 axisvalue[4]={0};
        S32 buttonstat[11]={0};
        S32 k=0;
        while(k<10)
        {
            Joyst.Update();

            axiscount=Joyst.GetAxisCount();
            for(S32 i=0; i<4; i++)
            {
                axisvalue[i]=Joyst.GetAxisValue(i);
            }
            buttoncount=Joyst.GetButtonCount();
            for(S32 i=0; i<buttoncount; i++)
            {
                buttonstat[i]=Joyst.GetButtonStatus(i);
            }


            printf("\naxis count :%d\naxis value:",axiscount);
            for(S32 i=0; i<4; i++)
            {
                printf("%f\t",axisvalue[i]);
            }
            printf("\nbutton count:%d\n",buttoncount);
            for(S32 i=0; i<buttoncount; i++)
            {
                printf("%d\t",buttonstat[i]);
            }
            printf("\npause\n");
            for(S32 i=0; i<10000000; i++);
        }
    }
    else
        printf("fail to open!\n");

}
