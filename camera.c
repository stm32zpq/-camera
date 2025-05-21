/*
���ߣ�Charon and ������С��Ƭ
       δ����Ȩ��ֹת��
 */
#include "sys.h"

int piancha=0;
uint8 xunxian =0;
uint8 bend_straight_flag=0;


//--------------------------
//����ͷǰհ������������ͷλ�����ã����ִ���Ȩ�ش�С�������1����Ϊ1�����0�е�Ȩ��Ϊ1
int weight[120]={
2,2,2,2,2,2,2,2,2,2,
2,2,2,2,2,2,2,2,2,2,//21-30
4,4,4,4,4,4,4,4,4,4,//1-10
8,8,8,8,8,8,8,8,8,8,//11-20
10,10,10,10,10,10,10,10,10,10,//21-30
12,12,12,12,12,12,12,12,12,12,//31-40
14,14,14,14,14,14,14,14,14,14,//41-50
16,16,16,16,16,16,16,16,16,16,//51-60
10,10,10,10,10,10,10,10,10,10,//61-70
8,8,8,8,8,8,8,8,8,8,//71-80
4,4,4,4,4,4,4,4,4,4,//81-90
3,3,3,3,3,3,3,3,3,3,//91-100
};//11120




/*************==============================================================================================************
 * ����ƫ��ֵ���͸�PID���е���
 ***********************
 *************===============================================================================================***********/



#include "zf_common_headfile.h"

#define White 255  //����ͼ���
#define Black 0    //����ͼ���

#define Left 1
#define Right 2
int x,y;
float parameterB,parameterA;   //y=parameterB*x+parameterA
int left[120]={2};       //��߽�����   ��118��ʼ��  �洢��˳���ǵ�һ�оʹ���left[1]��      //��ȡ�����������ߵ�������+1
int right[120]={185};    //�ұ߽�����   ��118��ʼ��  �洢��˳���ǵ�һ�оʹ���right[1]��     //��ȡ�����������ߵ�������-1
int middle[120]={93};    //��������
int Endline=1;           //��ֹ��
int WhiteNum=0;
int X1,Y1;//���²��ߵ㣨Բ����
uint8 right_lost_num=0;        //�ұ߶�����
uint8 left_lost_num=0;         //��߶�����
uint8 S_right_lost_num=0;        //S�ұ߶�����
uint8 S_left_lost_num=0;         //S��߶�����
uint8 imag[120][188];          //��ֵ��ͼ������
uint8 threshold_value=175;
uint32 image_process_time=0;   //ͼ����ʱ��
//��������
uint8 left_2_growth_direction=0;    //�����������2�ĸ���
uint8 left_5_growth_direction=0;    //�����������5�ĸ���
uint8 right_2_growth_direction=0;   //�ұ���������2�ĸ���
uint8 right_5_growth_direction=0;   //�ұ���������5�ĸ���
uint8 l_growth_direction_flag=0;    //�����������2��־λ
uint8 r_growth_direction_flag=0;    //�ұ���������2��־λ

uint8 Right_straight_flag=0; //��ֱ��
uint8 Left_straight_flag=0; //��ֱ��

uint8 annulus_L_Flag=0;       //��Բ����־
uint8 annulus_R_Flag=0;       //��Բ����־
uint8 annulus_L_memory=0;     //��Բ��״̬
uint8 annulus_R_memory=0;     //��Բ��״̬

uint8 zebra_crossing_flag=0;//�����߱�־
uint8 Left_garage_flag=0;   //�󳵿��־
uint8 Left_garage_memory=0; //�󳵿�״̬

//������
int16 gyro_start=0;    //��������ʼֵ
int16 gyro_end=0;      //��������ʼֵ

//Բ��͹���
uint8 roundabout_X=0;    //Բ��͹���������
uint8 roundabout_Y=0;    //Բ��͹���������
uint8 roundabout_Flag=0; //Բ��͹����־

//����ʶ���
uint8 Exit_loop_X=0;    //Բ������������
uint8 Exit_loop_Y=0;    //Բ������������
uint8 Exit_loop_Flag=0; //Բ��������־

uint8 Crossroad_Flag=0;       //ʮ�ֱ�־
uint8 Crossroad_memory=0;     //ʮ��״̬
uint8 Finish_Flag=0;          //������ɱ�ʶλ
//����
uint8 Lost_left_Flag=0;         //��߶��߱�־
uint8 Lost_right_Flag=0;        //�ұ߶��߱�־
uint8 Lost_point_L_scan_line=0; //��߶���ɨ�����ڵڼ���
uint8 Lost_point_R_scan_line=0; //�ұ߶���ɨ�����ڵڼ���


//ʮ�ֹյ�
uint8 ten_inflexion_down_l=0;    //ʮ�����¹յ�������
uint8 ten_inflexion_down_r=0;    //ʮ�����¹յ�������
uint8 ten_inflexion_up_l=0;      //ʮ�����Ϲյ�������
uint8 ten_inflexion_up_r=0;      //ʮ�����Ϲյ�������

//ʮ�ֹյ��־
uint8 ten_inflexion_down_l_flag=0;    //ʮ�����¹յ�������
uint8 ten_inflexion_down_r_flag=0;    //ʮ�����¹յ�������
uint8 ten_inflexion_up_l_flag=0;      //ʮ�����Ϲյ�������
uint8 ten_inflexion_up_r_flag=0;      //ʮ�����Ϲյ�������

//���¹յ�
uint8 Lower_left_inflection_X =0;      //���¹յ�������
uint8 Lower_left_inflection_Y =0;      //���¹յ�������
uint8 Lower_left_inflection_Flag=0;    //���¹յ��־

//���¹յ�
uint8 Lower_right_inflection_X =0;      //���¹յ�������
uint8 Lower_right_inflection_Y =0;      //���¹յ�������
uint8 Lower_right_inflection_Flag=0;    //���¹յ��־

//���Ϲյ�
uint8 Upper_left_inflection_X =0;      //���Ϲյ�������
uint8 Upper_left_inflection_Y =0;      //���Ϲյ�������
uint8 Upper_left_inflection_Flag=0;    //���Ϲյ��־

//���Ϲյ�
uint8 Upper_right_inflection_X =0;      //���Ϲյ�������
uint8 Upper_right_inflection_Y =0;      //���Ϲյ�������
uint8 Upper_right_inflection_Flag=0;    //���Ϲյ��־

//S����־
uint8 S_left_lost_Flag=0;               //���s�����߱�־
uint8 S_right_lost_Flag=0;              //�ұ�s�����߱�־
uint8 S_left_inflection_Flag=0;         //���s����־
uint8 S_right_inflection_Flag=0;        //�ұ�s����־
uint8 S_road_Flag=0;                    //s����־
uint8 S_road_memory=0;                  //s��״̬

uint8 road_width[120]={   0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  //�������
                          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                          138, 138, 141, 141, 144, 144, 147, 147, 150, 150 };

/*=========================================����ֵ����=======================================================*/
float absolute(float z)
{
    z = z< 0 ? (-z) : z;
    return z;
}
/*================================================================================================*/
int16 limit_a_b(int x, int a, int b)
{
    if(x<a) x = a;
    if(x>b) x = b;
    return (int16)x;
}


int16 limit1(int16 x, int16 y)
{
    if (x > y)             return y;
    else if (x < -y)       return -y;
    else                   return x;
}


uint8 original_image[image_h][image_w];     //ת��ͼ������
/*=========================================����ͼ��ת��========================================================*/
void Get_image(uint8(*mt9v03x_image)[image_w])
{
#define use_num     1   //1 ��ѹ����2 ѹ��һ��
    uint8 i = 0, j = 0, row = 0, line = 0;
    for (i = 0; i < image_h; i += use_num)          //
    {
        for (j = 0; j <image_w; j += use_num)     //
        {
            original_image[row][line] = mt9v03x_image[i][j];//����Ĳ�����д�������ͷ�ɼ�����ͼ��
            line++;
        }
        line = 0;
        row++;
    }
}
/*=======================================����ͼ����========================================================*/
uint8 OtsuThreshold(uint8 *image, uint16 col, uint16 row)
{

#define GrayScale 256
    uint16 Image_Width  = col;
    uint16 Image_Height = row;
    int X; uint16 Y;
    uint8* data = image;
    int HistGram[GrayScale] = {0};

    uint32 Amount = 0;
    uint32 PixelBack = 0;
    uint32 PixelIntegralBack = 0;
    uint32 PixelIntegral = 0;
    int32 PixelIntegralFore = 0;
    int32 PixelFore = 0;
    double OmegaBack=0, OmegaFore=0, MicroBack=0, MicroFore=0, SigmaB=0, Sigma=0; // ��䷽��;
    uint8 MinValue=0, MaxValue=0;
    uint8 Threshold = 0;


    for (Y = 0; Y <Image_Height; Y++) //Y<Image_Height��ΪY =Image_Height���Ա���� �ж�ֵ��
    {
        //Y=Image_Height;
        for (X = 0; X < Image_Width; X++)
        {
        HistGram[(int)data[Y*Image_Width + X]]++; //ͳ��ÿ���Ҷ�ֵ�ĸ�����Ϣ
        }
    }




    for (MinValue = 0; MinValue < 256 && HistGram[MinValue] == 0; MinValue++) ;        //��ȡ��С�Ҷȵ�ֵ
    for (MaxValue = 255; MaxValue > MinValue && HistGram[MinValue] == 0; MaxValue--) ; //��ȡ���Ҷȵ�ֵ

    if (MaxValue == MinValue)
    {
        return MaxValue;          // ͼ����ֻ��һ����ɫ
    }
    if (MinValue + 1 == MaxValue)
    {
        return MinValue;      // ͼ����ֻ�ж�����ɫ
    }

    for (Y = MinValue; Y <= MaxValue; Y++)
    {
        Amount += HistGram[Y];        //  ��������
    }

    PixelIntegral = 0;
    for (Y = MinValue; Y <= MaxValue; Y++)
    {
        PixelIntegral += HistGram[Y] * Y;//�Ҷ�ֵ����
    }
    SigmaB = -1;
    for (Y = MinValue; Y < MaxValue; Y++)
    {
          PixelBack = PixelBack + HistGram[Y];    //ǰ�����ص���
          PixelFore = Amount - PixelBack;         //�������ص���
          OmegaBack = (double)PixelBack / Amount;//ǰ�����ذٷֱ�
          OmegaFore = (double)PixelFore / Amount;//�������ذٷֱ�
          PixelIntegralBack += HistGram[Y] * Y;  //ǰ���Ҷ�ֵ
          PixelIntegralFore = PixelIntegral - PixelIntegralBack;//�����Ҷ�ֵ
          MicroBack = (double)PixelIntegralBack / PixelBack;//ǰ���ҶȰٷֱ�
          MicroFore = (double)PixelIntegralFore / PixelFore;//�����ҶȰٷֱ�
          Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore);//g
          if (Sigma > SigmaB)//����������䷽��g
          {
              SigmaB = Sigma;
              Threshold = (uint8)Y;
          }
    }

   return Threshold;
}
/*=======================================����ͼ���ֵ��========================================================*/
void binaryzation(void)
{
  uint8 i,j;

threshold_value = OtsuThreshold(original_image[0], image_w, image_h)+5;
  for(i = 0;i<image_h;i++)
  {

      for(j = 0;j<image_w;j++)
      {
          if(original_image[i][j]>threshold_value)
              imag[i][j] = white_pixel;
          else
              imag[i][j] = black_pixel;

      }

  }

}

/*=======================================Ѱ���������========================================================*/
uint8 start_point_l[2] = { 0 };//�������x��yֵ
uint8 start_point_r[2] = { 0 };//�ұ�����x��yֵ
uint8 get_start_point(uint8 start_row)
{
    uint8 i = 0,l_found = 0,r_found = 0;
    //����
    start_point_l[0] = 0;//x
    start_point_l[1] = 0;//y

    start_point_r[0] = 0;//x
    start_point_r[1] = 0;//y

        //���м�����ߣ��������
    for (i = image_w / 2; i > border_min; i--)
    {
        start_point_l[0] = i;//x
        start_point_l[1] = start_row;//y
        if (imag[start_row][i] == 255 && imag[start_row][i - 1] == 0)
        {
            //printf("�ҵ�������image[%d][%d]\n", start_row,i);
            l_found = 1;
            break;
        }
    }

    for (i = image_w / 2; i < border_max; i++)
    {
        start_point_r[0] = i;//x
        start_point_r[1] = start_row;//y
        if (imag[start_row][i] == 255 && imag[start_row][i + 1] == 0)
        {
            //printf("�ҵ��ұ����image[%d][%d]\n",start_row, i);
            r_found = 1;
            break;
        }
    }

    if(l_found&&r_found)return 1;
    else {
        //printf("δ�ҵ����\n");
        return 0;
    }
}

/*=======================================����ͼ�������========================================================*/
#define USE_num image_h*3
 //��ŵ��x��y����
uint16 points_l[(uint16)USE_num][2] = { {  0 } };//����      points_l[num][0]������������  points_l[num][1]������������
uint16 points_r[(uint16)USE_num][2] = { {  0 } };//����      points_r[num][0]������������  points_r[num][1]������������
uint16 dir_r[(uint16)USE_num] = { 0 };//�����洢�ұ���������
uint16 dir_l[(uint16)USE_num] = { 0 };//�����洢�����������
uint16 data_stastics_l = 0;//ͳ������ҵ���ĸ���
uint16 data_stastics_r = 0;//ͳ���ұ��ҵ���ĸ���
void search_l_r(uint16 break_flag, uint8(*image)[image_w], uint16 *l_stastic, uint16 *r_stastic, uint8 l_start_x, uint8 l_start_y, uint8 r_start_x, uint8 r_start_y, int*Endline)
{

    uint8 i = 0, j = 0;
    //��߱���
    uint8 search_filds_l[8][2] = { {  0 } };
    uint8 index_l = 0;
    uint8 temp_l[8][2] = { {  0 } };
    uint8 center_point_l[2] = {  0 };
    uint16 l_data_statics;//ͳ�����
    //����˸�����
    static int8 seeds_l[8][2] = { {0,  1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1},{1,  0},{1, 1}, };
    //{-1,-1},{0,-1},{+1,-1},
    //{-1, 0},       {+1, 0},
    //{-1,+1},{0,+1},{+1,+1},
    //�����˳ʱ��

    //�ұ߱���
    uint8 search_filds_r[8][2] = { {  0 } };
    uint8 center_point_r[2] = { 0 };//���������
    uint8 index_r = 0;//�����±�
    uint8 temp_r[8][2] = { {  0 } };
    uint16 r_data_statics;//ͳ���ұ�
    //����˸�����
    static int8 seeds_r[8][2] = { {0,  1},{1,1},{1,0}, {1,-1},{0,-1},{-1,-1}, {-1,  0},{-1, 1}, };
    //{-1,-1},{0,-1},{+1,-1},
    //{-1, 0},       {+1, 0},
    //{-1,+1},{0,+1},{+1,+1},
    //�������ʱ��

    l_data_statics = *l_stastic;//ͳ���ҵ��˶��ٸ��㣬��������ѵ�ȫ��������
    r_data_statics = *r_stastic;//ͳ���ҵ��˶��ٸ��㣬��������ѵ�ȫ��������

    //��һ�θ��������  ���ҵ������ֵ������
    center_point_l[0] = l_start_x;//x
    center_point_l[1] = l_start_y;//y
    center_point_r[0] = r_start_x;//x
    center_point_r[1] = r_start_y;//y

        //��������ѭ��
    while (break_flag--)
    {

        //���
        for (i = 0; i < 8; i++)//����8F����
        {
            search_filds_l[i][0] = center_point_l[0] + seeds_l[i][0];//x
            search_filds_l[i][1] = center_point_l[1] + seeds_l[i][1];//y
        }
        //�����������䵽�Ѿ��ҵ��ĵ���
        points_l[l_data_statics][0] = center_point_l[0];//x
        points_l[l_data_statics][1] = center_point_l[1];//y
        l_data_statics++;//������һ

        //�ұ�
        for (i = 0; i < 8; i++)//����8F����
        {
            search_filds_r[i][0] = center_point_r[0] + seeds_r[i][0];//x
            search_filds_r[i][1] = center_point_r[1] + seeds_r[i][1];//y
        }
        //�����������䵽�Ѿ��ҵ��ĵ���
        points_r[r_data_statics][0] = center_point_r[0];//x
        points_r[r_data_statics][1] = center_point_r[1];//y

        index_l = 0;//�����㣬��ʹ��
        for (i = 0; i < 8; i++)
        {
            temp_l[i][0] = 0;//�����㣬��ʹ��
            temp_l[i][1] = 0;//�����㣬��ʹ��
        }

        //����ж�
        for (i = 0; i < 8; i++)
        {
            if (image[search_filds_l[i][1]][search_filds_l[i][0]] == 0
                && image[search_filds_l[(i + 1) & 7][1]][search_filds_l[(i + 1) & 7][0]] == 255)
            {
                temp_l[index_l][0] = search_filds_l[(i)][0];
                temp_l[index_l][1] = search_filds_l[(i)][1];
                index_l++;
                dir_l[l_data_statics - 1] = (i);//��¼��������
            }

            if (index_l)
            {
                //���������
                center_point_l[0] = temp_l[0][0];//x
                center_point_l[1] = temp_l[0][1];//y
                for (j = 0; j < index_l; j++)
                {
                    if (center_point_l[1] > temp_l[j][1])
                    {
                        center_point_l[0] = temp_l[j][0];//x
                        center_point_l[1] = temp_l[j][1];//y
                    }
                }
            }

        }
        if ((points_r[r_data_statics][0]== points_r[r_data_statics-1][0]&& points_r[r_data_statics][0] == points_r[r_data_statics - 2][0]
            && points_r[r_data_statics][1] == points_r[r_data_statics - 1][1] && points_r[r_data_statics][1] == points_r[r_data_statics - 2][1])
            ||(points_l[l_data_statics-1][0] == points_l[l_data_statics - 2][0] && points_l[l_data_statics-1][0] == points_l[l_data_statics - 3][0]
                && points_l[l_data_statics-1][1] == points_l[l_data_statics - 2][1] && points_l[l_data_statics-1][1] == points_l[l_data_statics - 3][1]))
        {
            //printf("���ν���ͬһ���㣬�˳�\n");
            break;
        }
        if (absolute(points_r[r_data_statics][0] - points_l[l_data_statics - 1][0]) < 2
            && absolute(points_r[r_data_statics][1] - points_l[l_data_statics - 1][1] < 2)
            )
        {
            //printf("\n���������˳�\n");
            *Endline = (points_r[r_data_statics][1] + points_l[l_data_statics - 1][1]) >> 1;//ȡ����ߵ�
            //printf("\n��y=%d���˳�\n",*Endline);
            break;
        }
        if ((points_r[r_data_statics][1] < points_l[l_data_statics - 1][1]))
        {
           // printf("\n�����߱��ұ߸��ˣ���ߵȴ��ұ�\n");
            continue;//�����߱��ұ߸��ˣ���ߵȴ��ұ�
        }
        if (dir_l[l_data_statics - 1] == 7
            && (points_r[r_data_statics][1] > points_l[l_data_statics - 1][1]))//��߱��ұ߸����Ѿ�����������
        {
            //printf("\n��߿�ʼ�����ˣ��ȴ��ұߣ��ȴ���... \n");
            center_point_l[0] = (uint8)points_l[l_data_statics - 1][0];//x
            center_point_l[1] = (uint8)points_l[l_data_statics - 1][1];//y
            l_data_statics--;
        }
        r_data_statics++;//������һ

        index_r = 0;//�����㣬��ʹ��
        for (i = 0; i < 8; i++)
        {
            temp_r[i][0] = 0;//�����㣬��ʹ��
            temp_r[i][1] = 0;//�����㣬��ʹ��
        }

        //�ұ��ж�
        for (i = 0; i < 8; i++)
        {
            if (image[search_filds_r[i][1]][search_filds_r[i][0]] == 0
                && image[search_filds_r[(i + 1) & 7][1]][search_filds_r[(i + 1) & 7][0]] == 255)
            {
                temp_r[index_r][0] = search_filds_r[(i)][0];
                temp_r[index_r][1] = search_filds_r[(i)][1];
                index_r++;//������һ
                dir_r[r_data_statics - 1] = (i);//��¼��������
                //printf("dir[%d]:%d\n", r_data_statics - 1, dir_r[r_data_statics - 1]);
            }
            if (index_r)
            {

                //���������
                center_point_r[0] = temp_r[0][0];//x
                center_point_r[1] = temp_r[0][1];//y
                for (j = 0; j < index_r; j++)
                {
                    if (center_point_r[1] > temp_r[j][1])
                    {
                        center_point_r[0] = temp_r[j][0];//x
                        center_point_r[1] = temp_r[j][1];//y
                    }
                }

            }
        }


    }

    //ȡ��ѭ������
    *l_stastic = l_data_statics;
    *r_stastic = r_data_statics;

}
/*=======================================����������������ȡ========================================================*/
uint8 r1=0,r2=0,r3=0,r4=0,r5=0,r6=0,r7=0,r8=0;
uint8 l1=0,l2=0,l3=0,l4=0,l5=0,l6=0,l7=0,l8=0;
void growth_direction(void)
{
    uint16 i;
    r1=0;
    r2=0;
    r3=0;
    r4=0;
    r5=0;
    r6=0;
    r7=0;
    r8=0;

    l1=0;
    l2=0;
    l3=0;
    l4=0;
    l5=0;
    l6=0;
    l7=0;
    l8=0;

    /*===����===*/
    left_2_growth_direction=0;
    right_2_growth_direction=0;
    /*===�����ȡ�����������===*/
    for(i=0;i<=data_stastics_l;i++)
    {
        if(dir_l[i]==2)
            left_2_growth_direction++;
        if(dir_l[i]==5)
            left_5_growth_direction++;
        if(dir_l[i]==1)
            l1++;
        if(dir_l[i]==2)
            l2++;
        if(dir_l[i]==3)
            l3++;
        if(dir_l[i]==4)
            l4++;
        if(dir_l[i]==5)
            l5++;
        if(dir_l[i]==6)
            l6++;
        if(dir_l[i]==7)
            l7++;
        if(dir_l[i]==0)
            l8++;
    }
    /*===�ұ���ȡ�����������===*/
    for(i=0;i<=data_stastics_r;i++)
    {
        if(dir_r[i]==2)
          right_2_growth_direction++;
        if(dir_r[i]==5)
          right_5_growth_direction++;
        if(dir_r[i]==1)
            r1++;
        if(dir_r[i]==2)
            r2++;
        if(dir_r[i]==3)
            r3++;
        if(dir_r[i]==4)
            r4++;
        if(dir_r[i]==5)
            r5++;
        if(dir_r[i]==6)
            r6++;
        if(dir_r[i]==7)
            r7++;
        if(dir_r[i]==0)
            r8++;
    }


    /*����ж�*/
    if(left_2_growth_direction>30&&left_5_growth_direction>30)
    {
        l_growth_direction_flag=1;
    }
    else
    {
        l_growth_direction_flag=0;
    }

    /*�ұ��ж�*/
    if(right_2_growth_direction>30&&right_5_growth_direction>30)
    {
        r_growth_direction_flag=1;
    }
    else
    {
        r_growth_direction_flag=0;
    }
}

/*=======================================�����������ȡ========================================================*/
void get_left(uint16 total_L)
{
    uint8 i = 0;
    uint16 j = 0;
    uint8 h = 0;

    //��ʼ��
    for (i = 0;i<image_h;i++)
    {
        left[i] = border_min;

    }
    h = image_h - 2;               //��118�п�ʼ��ȡ
    //���
    for (j = 0; j < total_L; j++)
    {
        //printf("%d\n", j);
        if (points_l[j][1] == h)
        {
            left[h] = points_l[j][0]+1;   //��ȡ�����ڱ߽��ߵ��ڲ�

        }
        else continue; //ÿ��ֻȡһ���㣬û����һ�оͲ���¼
        h--;
        if (h == 0)
        {
            break;//�����һ���˳�
        }
    }
}
/*===========================================�������·������ж�====================================================*/
void lost_left(void){
    uint8 i=0;
    left_lost_num=0;
    Lost_left_Flag=0;
    S_left_lost_num=0;
    S_left_lost_Flag=0;
    for(i=110;i>10;i--)
    {
        if(imag[i][2]==White)     //��ߵ�2�е��ڰ׾Ͷ���
        {
            left_lost_num++;
            Lost_point_L_scan_line=i+4;
        }
        if(left_lost_num>10)
        {
            Lost_left_Flag=1; //�ж�����·��Ƿ���
            break;
//            return;
        }
    }

    for(i=110;i>10;i--)
    {
        if(imag[i][2]==White)     //��ߵ�2�е��ڰ׾Ͷ���
        {
            S_left_lost_num++;
        }
        if(S_left_lost_num>10)
        {
            S_left_lost_Flag=1; //�ж�����·��Ƿ���
            break;
//            return;
        }
    }

}
/*=======================================�����ұ�����ȡ========================================================*/
void get_right(uint16 total_R)
{
    uint8 i = 0;
    uint16 j = 0;
    uint8 h = 0;

    for (i = 0; i < image_h; i++)
    {
        right[i] = border_max;//�ұ��߳�ʼ���ŵ����ұߣ�����߷ŵ�����ߣ�����������պ�����������߾ͻ����м䣬������ŵõ�������

    }
    h = image_h - 2;
    //�ұ�
    for (j = 0; j < total_R; j++)
    {
        if (points_r[j][1] == h)
        {
            right[h] = points_r[j][0] - 1;

        }
        else continue;//ÿ��ֻȡһ���㣬û����һ�оͲ���¼
        h--;
        if (h == 0)break;//�����һ���˳�
    }
}
/*===========================================�������·������ж�====================================================*/
void lost_right(void){
    uint8 i=0;
    right_lost_num=0;
    Lost_right_Flag=0;
    S_right_lost_num=0;
    S_right_lost_Flag=0;
    for(i=110;i>10;i--)
    {
        if(imag[i][185]==White)     //�ұߵ�185�е��ڰ׾Ͷ���
        {
            right_lost_num++;
            Lost_point_R_scan_line=i+4;
        }
        if(right_lost_num>15)
        {
            Lost_right_Flag=1;  //�ж��ұ��·��Ƿ���
            break;
//            return;
        }
    }
    for(i=110;i>10;i--)
    {
        if(imag[i][185]==White)     //�ұߵ�185�е��ڰ׾Ͷ���
        {
            S_right_lost_num++;
        }
        if(S_right_lost_num>10)
        {
            S_right_lost_Flag=1;  //�ж��ұ��·��Ƿ���
            break;
//            return;
        }
    }
}
/*===========================================����������ȡ===================================================*/
void middle_line(void){
    for(y=119;y>Endline;y--)           //��Ч��ȫ����ȡ����
    {
        middle[y]=(right[y]+left[y])/2;
    }
}
/*===========================================�������ͺ͸�ʴ====================================================*/
#define threshold_max   255*5
#define threshold_min   255*2
void image_filter(uint8(*imag)[image_w])//��̬ѧ�˲������ͺ͸�ʴ��˼��
{
    uint16 i, j;
    uint32 num = 0;


    for (i = 1; i < image_h - 1; i++)
    {
        for (j = 1; j < (image_w - 1); j++)
        {
            //ͳ�ư˸����������ֵ
            num =
                imag[i - 1][j - 1] + imag[i - 1][j] + imag[i - 1][j + 1]
                + imag[i][j - 1] + imag[i][j + 1]
                + imag[i + 1][j - 1] + imag[i + 1][j] + imag[i + 1][j + 1];


            if (num >= threshold_max && imag[i][j] == 0)
            {

                imag[i][j] = 255;//��  ���Ը�ɺ궨�壬�������

            }
            if (num <= threshold_min && imag[i][j] == 255)
            {

                imag[i][j] = 0;//��

            }

        }
    }

}

/*===========================================���л��ڿ�====================================================*/
void image_draw_rectan(uint8(*image)[image_w])
{

    uint8 i = 0;
    for (i = 0; i < image_h; i++)     //�����0��1�к��ұ�186��187�л��ڿ�
    {
        image[i][0] = 0;
        image[i][1] = 0;
        image[i][image_w - 1] = 0;
        image[i][image_w - 2] = 0;

    }
    for (i = 0; i < image_w; i++)     //���Ϸ�0��1�л��ڿ�
    {
        image[0][i] = 0;
        image[1][i] = 0;
        //image[image_h-1][i] = 0;

    }
}


//==========================================================�յ�ʶ��====================================================

/*===========================================ʮ���жϹյ�====================================================*/
void ten_inflection(void)
{
    uint8 i;
    /*====����====*/
    //ʮ�ֹյ�
    ten_inflexion_down_l=0;    //ʮ�����¹յ�������
    ten_inflexion_down_r=0;    //ʮ�����¹յ�������
    ten_inflexion_up_l=0;      //ʮ�����Ϲյ�������
    ten_inflexion_up_r=0;      //ʮ�����Ϲյ�������
    //ʮ�ֹյ��־
    ten_inflexion_down_l_flag=0;    //ʮ�����¹յ�������
    ten_inflexion_down_r_flag=0;    //ʮ�����¹յ�������
    ten_inflexion_up_l_flag=0;      //ʮ�����Ϲյ�������
    ten_inflexion_up_r_flag=0;      //ʮ�����Ϲյ�������
    /*====����====*/


    /*=====ʮ���¹յ��ж�=====*/
    /*���¹յ��ж�*/
    for(i=image_h-20;i>40;i--)
    {
        if((left[i]-left[i-1]<3)&&(left[i-1]-left[i-2]<3)&&(left[i-3]-left[i-4]>3))
        {
          ten_inflexion_down_l=i;
          ten_inflexion_down_l_flag=1;
          break;
        }

    }
    /*���¹յ��ж�*/
    for(i=image_h-20;i>40;i--)
    {
        if((right[i]-right[i-1]<3)&&(right[i-2]-right[i-1]<3)&&(right[i-4]-right[i-3]>3))
        {
           ten_inflexion_down_r=i;
           ten_inflexion_down_r_flag=1;
           break;
        }

    }
    /*=====ʮ���Ϲյ��ж�=====*/
    /*���Ϲյ��ж�*/
    for(i=image_h-15;i>60;i--)
    {
        if((left[i]-left[i+1]<3)&&(left[i+1]-left[i+2]<3)&&(left[i+3]-left[i+4]>3))
        {
            ten_inflexion_up_l=i;
            ten_inflexion_up_l_flag=1;
            break;
        }

    }
    /*���Ϲյ��ж�*/
    for(i=image_h-15;i>60;i--)
    {
        if((right[i+1]-right[i]<3)&&(right[i+2]-right[i+1]<3)&&(right[i+4]-right[i+3]>3))
        {
           ten_inflexion_up_r=i;
           ten_inflexion_up_r_flag=1;
           break;
        }

    }
}
//---------------------------------���¹յ�--------------------------------
//-----------------------------�ڶ��棺�öϵ��ж�------------------------
void Lower_left(void){
    Lower_left_inflection_Flag=0;
    Lower_left_inflection_X =0;
    Lower_left_inflection_Y =0;

        for(y=image_h-3;y>Endline+10;y--){
            if(y>30){
                if((left[y]-left[y-4])>5&&left[y-4]==2&&(left[y]-left[y+2])<5&&left[y]>10)
                {
                    Lower_left_inflection_Flag=1;
                    Lower_left_inflection_X =(uint8)left[y];
                    Lower_left_inflection_Y =(uint8)y;
                    return;
                }
            }

         }
    //----------------------------��һ�棺��ɨ�߷����ж�--------------------------------
    //    for(y=110;y>(Endline+10);y--){
    //        for(x=left[y+1];x<186&&x>1;x++){
    //              if(imag[y][x-1]==Black&&imag[y][x]==Black&&imag[y][x+1]==White){
    //                 Lower_left_inflection_X =x;
    //                 Lower_left_inflection_Y =y;
    //              //  ips200_draw_point(Lower_left_inflection_X, Lower_left_inflection_Y , RGB565_RED);
    //                 break;
    //              }
    //              if(x>(left[y-1]+3)&&imag[y][x]==White){
    //                  Lower_left_inflection_Flag=1;
    //                  X1=Lower_left_inflection_X;
    //                  Y1=Lower_left_inflection_Y;
    //                  break;
    //                  }
    //            }
    //
    //
    //        }
}

//-------------------------------------���¹յ�---------------------------------
//-----------------------------�ڶ��棺�öϵ��ж�------------------------
void Lower_right(void){
    Lower_right_inflection_Flag=0;
    Lower_right_inflection_X =0;
    Lower_right_inflection_Y =0;
    for(y=image_h-3;y>(Endline+10);y--){
        if(y>30){
            if((right[y-4]-right[y])>5&&right[y-4]==185&&(right[y+2]-right[y])<5&&right[y]<177){
                Lower_right_inflection_Flag=1;
                Lower_right_inflection_X =(uint8)right[y];
                Lower_right_inflection_Y =(uint8)y;
                return;
            }
        }

     }
    //----------------------------��һ�棺��ɨ�߷����ж�--------------------------------
    //���ɶ����¹յ�ʶ�𣬵�����ת��ʱ�Ĺս�Ҳ�ᱻ�ж���
    //        for(y=115;y>(Endline+15);y--){
    //            for(x=right[y+1];x<186&&x>1;x--){
    //                  if(imag[y][x-1]==White&&imag[y][x]==Black&&imag[y][x+1]==Black){
    //                     Lower_right_inflection_X =x;
    //                     Lower_right_inflection_Y =y;
    //                    // ips200_draw_point(Lower_right_inflection_X, Lower_right_inflection_Y , RGB565_RED);
    //                     break;
    //                  }
    //                  if(x<(right[y-1]-3)&&imag[y][x]==White){
    //                      Lower_right_inflection_Flag=1;
    //                      break;
    //                      }
    //                }
    //
    //            }

}


//-------------------------------------���Ϲյ�------------------------------------
//-----------------------------�ڶ��棺�öϵ��ж�------------------------
void Upper_left(void){
    uint8 h=image_h-3;   //h=117��Ҫ�Ǹ��������Ϲյ��õ�
//    uint8 i;

    Upper_left_inflection_Flag=0;
    Upper_left_inflection_X =0;
    Upper_left_inflection_Y =0;
    if(Lost_left_Flag==1){
        //���Բ��д���ҵ㷽ʽ
          if(annulus_L_Flag==1)
          {
              for(h=Lost_point_L_scan_line+5;h>(Endline+10);h--)
              {
                if((left[h]-left[h+8])>3&&left[h+8]==2&&left[h]!=2&&(left[h-4]-left[h])<3&&h<60&&left[h]>10)
                {
                   if(left[h]>93)
                   {            //���Բ��״̬4���߳��ֵĶϲ�������еĳ����Ż�
                       Upper_left_inflection_Flag=1;
                       Upper_left_inflection_X =(uint8)left[h];
                       Upper_left_inflection_Y =h;

                       //-----------��һ�棺��ɨ�߷����ж�-----------
                       uint8 Find_Flag=0;
                       for(y=h;y<110;y++){
                           Find_Flag=0;
                           for(x=left[h]+10;x>70;x--)
                           {
                                 if(imag[y][x-1]==Black&&imag[y][x]==Black&&imag[y][x+1]==White)
                                 {
                                     Find_Flag=1;
                                    break;
                                 }

                           }
                           if(Find_Flag==0)
                           {
                               Upper_left_inflection_Y =y-1;
                               return;
                               }

                           }

                   }
                   else{
                       Upper_left_inflection_Flag=1;
                       Upper_left_inflection_X =(uint8)left[h];
                       Upper_left_inflection_Y =h;
                       return;
                   }
                }
             }
          }
          //���ǳ�������Ϲյ�
          else{
              for(h=Lost_point_L_scan_line+3;h>(Endline+10);h--){
                if((left[h]-left[h+4])>3&&left[h+10]==2&&left[h]!=2&&(left[h-1]-left[h])<3&&h<60&&left[h]>10){

                       Upper_left_inflection_Flag=1;
                       Upper_left_inflection_X =(uint8)left[h];
                       Upper_left_inflection_Y =h;
                       return;

                }
             }
          }

    }
    //----------------------------��һ�棺��ɨ�߷����ж�--------------------------------
    //        for(y=Endline+15;y<110;y++){
    //            for(x=left[y-1];x<186&&x>1;x--){
    //                  if(imag[y][x-1]==Black&&imag[y][x]==Black&&imag[y][x+1]==White){
    //                     Upper_left_inflection_X =x;
    //                     Upper_left_inflection_Y =y;
    //                     //ips200_draw_point(Upper_left_inflection_X, Upper_left_inflection_Y , RGB565_BLUE );
    //                     break;
    //                  }
    //                  if(x<(left[y-1]-3)){
    //                      Upper_left_inflection_Flag=1;
    //                      break;
    //                      }
    //                }
    //
    //
    //            }

}

//-----------------------------------���Ϲյ�-----------------------------------
//-----------------------------�ڶ��棺�öϵ��ж�------------------------
void Upper_right(void){
    uint8 h=image_h-3;
    Upper_right_inflection_Flag=0;
    Upper_right_inflection_X =0;
    Upper_right_inflection_Y =0;
    if(Lost_right_Flag==1){
        //���Բ��д���ҵ㷽ʽ
          if(annulus_R_Flag==1)
          {
              for(h=Lost_point_R_scan_line+5;h>(Endline+10);h--)
              {
                if((right[h+8]-right[h])>3&&right[h+8]==185&&right[h]!=185&&(right[h]-right[h-4])<3&&h<60&&right[h]<177)
                {
                   if(right[h]<93) {            //���Բ��״̬4���߳��ֵĶϲ�������еĳ����Ż�
                       Upper_right_inflection_Flag=1;
                       Upper_right_inflection_X =(uint8)right[h];
                       Upper_right_inflection_Y =h;
                       //-----------��һ�棺��ɨ�߷����ж�-----------
                       uint8 Find_Flag=0;
                       for(y=h;y<110;y++){
                           Find_Flag=0;
                           for(x=right[h]+10;x>70;x--){
                                 if(imag[y][x-1]==White&&imag[y][x]==Black&&imag[y][x+1]==Black){
                                     Find_Flag=1;
                                    break;
                                 }

                               }
                           if(Find_Flag==0){
                               Upper_right_inflection_Y =y-1;
                               return;
                               }

                           }

                   }
                   //û��Խ�����ߵ��Ϲյ�
                   else{
                       Upper_right_inflection_Flag=1;
                       Upper_right_inflection_X =(uint8)right[h];
                       Upper_right_inflection_Y =h;
                       return;
                   }
                }
             }
          }
          //�������Ϲյ�
          else{
              for(h=Lost_point_R_scan_line+5;h>(Endline+10);h--){

                      if((right[h+3]-right[h])>15&&right[h+10]==185&&right[h]!=185&&(right[h]-right[h-1])<3&&h<60&&right[h]<177){
                          Upper_right_inflection_Flag=1;
                          Upper_right_inflection_X =(uint8)right[h];
                          Upper_right_inflection_Y =h;
                          return;

                  }

              }
          }

    }
    //----------------------------��һ�棺��ɨ�߷����ж�--------------------------------
    //        for(y=Endline+15;y<110;y++){
    //            for(x=right[y-1];x<186&&x>1;x++){
    //                  if(imag[y][x-1]==White&&imag[y][x]==Black&&imag[y][x+1]==Black){
    //                     Upper_right_inflection_X =x;
    //                     Upper_right_inflection_Y =y;
    //                    // ips200_draw_point(Upper_right_inflection_X, Upper_right_inflection_Y , RGB565_BLUE );
    //                     break;
    //                  }
    //                  if(x>(right[y-1]+3)){
    //                      Upper_right_inflection_Flag=1;
    //                      break;
    //                      }
    //                }
    //            }
}
void left_S_road_inflection(void)
{
  uint8 i=0;
  S_left_inflection_Flag=0;
    for(y=image_h-3;y>Endline+10;y--)
    {
        if((left[y]-left[y-10])>3&&(left[y]-left[y+10])>3&&left[y]>10)
          {
            i++;
          }
        if(i>1)
          {
            S_left_inflection_Flag=1;
          }

    }
}
void right_S_road_inflection(void)
{
    uint8 i=0;
    S_right_inflection_Flag=0;
      for(y=image_h-3;y>Endline+10;y--)
      {
          if((right[y-10]-right[y])>3&&(right[y+10]-right[y])>3&&right[y]<177)
            {
              i++;
            }
          if(i>1)
            {
              S_right_inflection_Flag=1;
            }

      }
}

//--------�յ����ж�-----------
void inflection_point(void){

    Upper_left();
    Upper_right();
    Lower_left();
    Lower_right();
    left_S_road_inflection();
    right_S_road_inflection();

}


//==================================================��ֱ��ʶ��===========================================================
void right_straight(void){
        float k1,k2;
        Right_straight_flag=0;
        float l_slope2=0,l_slope3=0,l_distance2=0,l_distance3=0;
        caculate_distance(80,110,right,&l_slope3,&l_distance3);
        k2=l_slope3;
        caculate_distance(20,40,right,&l_slope2,&l_distance2);
        k1=l_slope2;
        if(absolute(k1-k2)<0.15)
        Right_straight_flag=1;


}

//==================================================��ֱ��ʶ��===========================================================
void left_straight(void)
{
    float k1,k2;
    Left_straight_flag=0;
    float l_slope2=0,l_slope3=0,l_distance2=0,l_distance3=0;
    caculate_distance(100,119,left,&l_slope3,&l_distance3);
    k2=l_slope3;
    caculate_distance(20,40,left,&l_slope2,&l_distance2);
    k1=l_slope2;
    if(absolute(k1-k2)<0.1)
    Left_straight_flag=1;

}


//=================================================ʮ��ʶ��=======================================================
void crossroad(void){

/*-----------------------�ڶ���ʮ�ֲ���--------------------------------*/
    int l;
    uint8 start,end;           //������ֱ�ߵĵ��������
    float slope=0,distance=0;  //���б�ʺͽؾ�
    if(annulus_L_Flag==0&&annulus_R_Flag==0&&Lost_left_Flag==1&&Lost_right_Flag==1&&Crossroad_Flag==0&&Crossroad_memory==0&&ten_inflexion_down_l_flag==1&&ten_inflexion_down_r_flag==1)
    {
       Crossroad_Flag=1;
       Crossroad_memory=1;
    }
    if(Crossroad_Flag==1)
    {
        /*==================��ʮ��·��ǰ���в���(����б�ʽؾຯ������С���˷�����)=======================*/
        if(Crossroad_memory==1)
        {
            /*===��߲���===*/
            start=ten_inflexion_down_l+3;
            end=ten_inflexion_down_l+15;
            caculate_distance(start,end,left,&slope,&distance);
            for(l=ten_inflexion_down_l;l>2;l--)
            {
                if((slope*(l)+distance)>185)
                    left[l]=185;
                else if((slope*(l)+distance)<2)
                    left[l]=2;
                else
                    left[l]=slope*(l)+distance;
            }
            /*===�ұ߲���===*/
            start=ten_inflexion_down_r+3;
            end=ten_inflexion_down_r+15;
            caculate_distance(start,end,right,&slope,&distance);
            for(l=ten_inflexion_down_r;l>2;l--)
            {
                if((slope*(l)+distance)>185)
                    right[l]=185;
                else if((slope*(l)+distance)<2)
                    right[l]=2;
                else
                    right[l]=slope*(l)+distance;
            }

            /*====�ж��Ƿ����״̬���Ϲյ㲹��===*/
            if(ten_inflexion_up_l_flag==1&&ten_inflexion_up_r_flag==1)
            {
                Crossroad_memory=2;
            }
        }

        /*============��ʮ��·�ں���в���(����б�ʽؾຯ������С���˷�����)===============*/
        if(Crossroad_memory==2)
        {
            /*===��߲���===*/
            start=ten_inflexion_up_l-15;
            end=ten_inflexion_up_l-3;
            caculate_distance(start,end,left,&slope,&distance);
            for(l=ten_inflexion_up_l;l<119;l++)
            {
                if((slope*(l)+distance)>185)
                    left[l]=185;
                else if((slope*(l)+distance)<2)
                    left[l]=2;
                else
                    left[l]=slope*(l)+distance;
            }
            /*===�ұ߲���===*/
            start=ten_inflexion_up_r-15;
            end=ten_inflexion_up_r-3;
            caculate_distance(start,end,right,&slope,&distance);
            for(l=ten_inflexion_up_r;l<119;l++)
            {
                if((slope*(l)+distance)>185)
                    right[l]=185;
                else if((slope*(l)+distance)<2)
                    right[l]=2;
                else
                    right[l]=slope*(l)+distance;
            }
            /*====�ж��Ƿ����ʮ��״̬==*/
            if(ten_inflexion_up_l_flag==0||ten_inflexion_up_r_flag==0)
            {
                Crossroad_Flag=0;
                Crossroad_memory=0;
            }
        }

    }

/*-----------------------��һ��ʮ�ֲ���--------------------------------*/
//    //��ǰʮ�ִ���
//    if(annulus_L_Flag==0&&annulus_R_Flag==0&&Lost_left_Flag==1&&Lost_right_Flag==1&&Lower_left_inflection_Flag==1&&Lower_right_inflection_Flag==1&&Crossroad_Flag==0&&Crossroad_memory==0&&Endline<5){
//        Crossroad_Flag=1;
//        Crossroad_memory=1;
//    }
//    if(Crossroad_Flag==1)
//    {
//        //״̬1 ����ʮ�֣����������������յ�
//       if(Crossroad_memory==1)
//       {
//          //����һ��ǿ�в���
////           Addingline( 1, 45,55, 3, 117);
////           Addingline( 2, 119,55, 186, 117);
//          //����������б�ʲ���
//
//           if(Lower_left_inflection_Flag==1)
//           {
//               Addingline1(1,Lower_left_inflection_X,Lower_left_inflection_Y);
//           }
//           else{
//               Addingline( 1, 45,55, 3, 117);
//           }
//
//           if(Lower_right_inflection_Flag==1){
//               Addingline1(2,Lower_right_inflection_X,Lower_right_inflection_Y);
//           }
//           else{
//               Addingline( 2, 119,55, 186, 117);
//           }
//
//        if ( Upper_left_inflection_Flag == 1 && Upper_right_inflection_Flag == 1)
//            {
//            Crossroad_memory=2;
//            }
//         }
//       //״̬2 �������Ϲյ�����Ϲյ㣬��������
//       if(Crossroad_memory==2){
//           if(Upper_left_inflection_Flag==1&&Upper_right_inflection_Flag==1){
//               Addingline( 1, Upper_left_inflection_X, Upper_left_inflection_Y, 5, 118);
//               Addingline( 2,Upper_right_inflection_X, Upper_right_inflection_Y, 184,118);
//           }
//           else{
//               Addingline( 1, 45,55, 3, 117);
//               Addingline( 2, 119,55, 186, 117);
//           }
//           if(Upper_left_inflection_Flag==0&&Upper_right_inflection_Flag==0){
//               Crossroad_memory=3;
//           }
//           else return;
//       }
//       //״̬3 ��ʮ������ʻ,�������յ㲹��
//       if(Crossroad_memory==3){
////           if(Lower_left_inflection_Flag==1){
////               Addingline1(1,Lower_left_inflection_X,Lower_left_inflection_Y);
////           }
////           if(Lower_right_inflection_Flag==1){
////               Addingline1(2,Lower_right_inflection_X,Lower_right_inflection_Y);
////           }
//           if(Upper_left_inflection_Flag==1&&Upper_right_inflection_Flag==1){
//               Crossroad_memory=4;
//           }
//       }
//       //״̬4 ���߳�ʮ��
//       if(Crossroad_memory==4){
//           if(Upper_left_inflection_Flag==1&&Upper_right_inflection_Flag==1){
//               Addingline( 1, Upper_left_inflection_X, Upper_left_inflection_Y, 5, 118);
//               Addingline( 2,Upper_right_inflection_X, Upper_right_inflection_Y, 184,118);
//           }
//           else{
//               Addingline( 1, 45,55, 3, 117);
//               Addingline( 2, 119,55, 186, 117);
//           }
//           //�˳�
//           if(Upper_left_inflection_Flag==0||Upper_right_inflection_Flag==0){
//               Crossroad_Flag=0;
//               Crossroad_memory=0;
//               return;
//           }
//       }
//
//  }
}

//=============================================Բ��===================================================
//================����ʶ�𣨻����м�͹���жϣ�=====================
void roundabout_L(void){

    roundabout_X=0;
    roundabout_Y=0;
    roundabout_Flag=0;
    for(y=image_h-3;y>10;y--){
        if((left[y]-left[y-8])>5&&left[y-8]==2&&(left[y]-left[y+2])<5&&left[115]==2&&left[117]==2&&Lost_left_Flag==1){
            y+=4;
            roundabout_Flag=1;
            roundabout_X =(uint8)left[y];
            roundabout_Y =(uint8)y;
            return;
        }
     }

}
//================����ʶ�𣨻����м�͹���жϣ�=====================
void roundabout_R(void){
    roundabout_X=0;
    roundabout_Y=0;
    roundabout_Flag=0;
    for(y=image_h-3;y>10;y--){
        if((right[y-8]-right[y])>5&&right[y-8]==185&&(right[y+2]-right[y])<5&&right[115]==185&&right[117]==185&&Lost_right_Flag==1){
            y+=4;
            roundabout_Flag=1;
            roundabout_X =(uint8)right[y];
            roundabout_Y =(uint8)y;
            return;
        }
     }

}
//================�����յ�ʶ��=====================
void Exit_loop_L_inflection(void){
   uint8 i;
   Exit_loop_Flag=0;
   Exit_loop_X=0;
   Exit_loop_Y=0;
  //��Բ����ʶ���ұ߹յ�
    for(i=110;i>Endline+10;i--){
        if(right[i]<right[i+4]&&right[i]<right[i-4]&&right[i]<right[i+3]&&right[i]<right[i-3]){
            Exit_loop_Flag=1;
            Exit_loop_X=(uint8)right[i];
            Exit_loop_Y=i;
            return;
        }
    }
}
//================�����յ�ʶ��=====================
void Exit_loop_R_inflection(void){
   uint8 i;
   Exit_loop_Flag=0;
   Exit_loop_X=0;
   Exit_loop_Y=0;
  //��Բ����ʶ���ұ߹յ�
    for(i=110;i>Endline+10;i--){
        if(left[i+4]<left[i]&&left[i-4]<left[i]&&left[i+3]<left[i]&&left[i-3]<left[i]){
            Exit_loop_Flag=1;
            Exit_loop_X=(uint8)left[i];
            Exit_loop_Y=i;
            return;
        }
    }
}

//================��Բ��ʶ��=====================
void annulus_L(void){

    //ʶ��Բ��
      if(l_growth_direction_flag==1&&annulus_R_Flag==0&&Crossroad_Flag==0&&Upper_left_inflection_Flag==0&&Lost_left_Flag==1&&Lower_left_inflection_Flag==1&&Right_straight_flag==1&&Lower_right_inflection_Flag==0&&annulus_L_memory==0&&annulus_L_Flag==0&&imag[Lower_left_inflection_Y][3]==White){
          annulus_L_Flag=1;
          annulus_L_memory =1;
          annulus_R_Flag=0;
          annulus_R_memory =0;
          Crossroad_Flag=0;
          Crossroad_memory=0;
      }

     if(annulus_L_Flag==1){

         //״̬ 1  ʶ��Բ����δʶ�𵽻�������ǿ�в��ߣ��򲻲��ߣ������ԣ�
         if (annulus_L_memory == 1)
         {
             if(Lower_left_inflection_Flag==1){
                 Addingline1( 1, Lower_left_inflection_X, Lower_left_inflection_Y);
                //Addingline( 1, Lower_left_inflection_X, Lower_left_inflection_Y,2 , 118 );
                //roundabout();
             }
             else if(Lower_left_inflection_Flag==0){
                 annulus_L_memory = 2;
             }
         }
         //״̬2 ʶ��Բ����������������߽��в���
          if (annulus_L_memory == 2 )
         {
              gyro_start=jd_sum;
              roundabout_L();          //����
            if(roundabout_Flag==1){
                Addingline( 1, roundabout_X, roundabout_Y,2 , 118 );

             }

            else {
                Addingline( 1, 56, 38,2 , 118 );
            }
            if(Upper_left_inflection_Flag==1&&Upper_left_inflection_Y>21){   //�ı�Upper_left_inflection_Y���Ƽ�Ӹı��뻷λ�ã�����֤
                annulus_L_memory = 3;
            }
            else return;
         }
         //״̬3 ����Բ����ڣ���סǰ·�������뻷
          if (annulus_L_memory == 3 )
          {
              if(Upper_left_inflection_Flag==1){
                  //Addingline( 2, Upper_left_inflection_X,Upper_left_inflection_Y,right[119] , 119);
                  if(Upper_left_inflection_X>25){
                      Addingline( 2, Upper_left_inflection_X-15,Upper_left_inflection_Y,(uint8)right[119] , 119);
                  }
                  else{
                      Addingline( 2, Upper_left_inflection_X,Upper_left_inflection_Y,(uint8)right[119] , 119);

                  }

                  return;
              }
              else if(Upper_left_inflection_Flag==0&&Lost_left_Flag==1&&Endline>30){
                  annulus_L_memory = 4;
              }
              else return;

          }
         //״̬4 ��Բ������ʻ�����������¹յ�ʱ������һ״̬
         if (annulus_L_memory == 4)
         {
            Exit_loop_L_inflection();
            if(Exit_loop_Flag==0){
                return;
            }
            if(Exit_loop_Flag==1&&Lost_left_Flag==1){
                annulus_L_memory = 5;
            }

         }

         //״̬5 ����ʱ�������¹յ�,���䲹�ߴ���
         if (annulus_L_memory == 5 )
          {
             Exit_loop_L_inflection();
             if(Exit_loop_Flag==1){
                 if(Exit_loop_Y>40){
                     Addingline( 2, 3, Exit_loop_Y-60, Exit_loop_X, Exit_loop_Y);//�˴����߽����������
                     return;
                 }
                 else {
                     Addingline( 2, 3, 50, Exit_loop_X, Exit_loop_Y);//�˴����߽����������
                     return;
                 }
             }
             else if(Lost_left_Flag==1&&Lost_right_Flag==1&&Exit_loop_Flag==0){
                 annulus_L_memory = 6;
             }
             else return;

          }

         //״̬6 ����ʱ���¹յ���ʧ�����ǳ���û��ȫ��������ʱ����Ҫ���ߴ���
         if (annulus_L_memory == 6 )
          {
             gyro_end=jd_sum;
             if(Lost_left_Flag==1&&Lost_right_Flag==1){
                 Addingline( 2, 3, Endline+5, 185, 117);
                // return;
             }
             if(((gyro_end-gyro_start)<-300)||(Lost_left_Flag==1&&Lost_right_Flag==0)){
                 annulus_L_memory = 7;
             }
             else return;
          }
         //״̬7 ��������
         if (annulus_L_memory == 7)
          {
             gyro_end=jd_sum;
             if(Upper_left_inflection_Flag==1){
                 Addingline( 1, Upper_left_inflection_X, Upper_left_inflection_Y, 10, 118);
                 return;
             }
             else{
                 Addingline( 1, 56, 38, 2, 118);
             }
             if((Lost_right_Flag==0&&Lost_left_Flag==0)/*||((gyro_end-gyro_start)<-300)*/)//||((gyro_end-gyro_start)<-300)
             {
                 annulus_L_memory =0;
                 annulus_L_Flag=0;
                 return;
             }

          }

     }
}

//================��Բ��ʶ��=====================
void annulus_R(void){

    //ʶ��Բ��
      if(r_growth_direction_flag==1&&annulus_L_Flag==0&&Crossroad_Flag==0&&Upper_right_inflection_Flag==0&&Lost_right_Flag==1&&Lower_right_inflection_Flag==1&&Left_straight_flag==1&&Lower_left_inflection_Flag==0&&annulus_R_memory==0&&annulus_R_Flag==0){
          annulus_R_Flag=1;
          annulus_R_memory =1;
          annulus_L_Flag=0;
          annulus_L_memory =0;
          Crossroad_Flag=0;
          Crossroad_memory=0;
      }

     if(annulus_R_Flag==1){


         //״̬ 1  ʶ��Բ����δʶ�𵽻�������ǿ�в��ߣ��򲻲��ߣ������ԣ�
         if (annulus_R_memory == 1)
         {
             if(Lower_right_inflection_Flag==1)
             {
//                 breadth_Addingline(2);
                 Addingline1( 2, Lower_right_inflection_X, Lower_right_inflection_Y);

                //Addingline( 1, Lower_left_inflection_X, Lower_left_inflection_Y,2 , 118 );
                //roundabout();
             }
             else if(Lower_left_inflection_Flag==0){
                 annulus_R_memory = 2;
             }


         }
         //״̬2 ʶ��Բ����������������߽��в���
          if (annulus_R_memory == 2 )
         {
              gyro_start=jd_sum;
              roundabout_R();          //����
            if(roundabout_Flag==1)
            {
//                breadth_Addingline(2);
                Addingline( 2, roundabout_X, roundabout_Y,187 , 118 );

             }
            else
            {
//                breadth_Addingline(2);
                Addingline( 2, 126, 38,187 , 118 );  //Ҫ��
            }
            if(Upper_right_inflection_Flag==1&&Upper_right_inflection_Y>19){   //�ı�Upper_right_inflection_Y���Ƽ�Ӹı��뻷λ�ã�����֤
                annulus_R_memory = 3;
            }
            else return;
         }
         //״̬3 ����Բ����ڣ���סǰ·�������뻷
          if (annulus_R_memory == 3 )
          {
              if(Upper_right_inflection_Flag==1){
                  //Addingline( 2, Upper_left_inflection_X,Upper_left_inflection_Y,right[119] , 119);
                  if(Upper_right_inflection_X<162)
                  {
                      Addingline( 1, Upper_right_inflection_X+40,Upper_right_inflection_Y,(uint8)left[119] , 119);
                  }
                  else{
                      Addingline( 1, Upper_right_inflection_X,Upper_right_inflection_Y,(uint8)left[119] , 119);

                  }

                  return;
              }
              else if(Upper_right_inflection_Flag==0&&Lost_right_Flag==1&&Endline>30){
                  annulus_R_memory = 4;
              }
              else return;

          }
         //״̬4 ��Բ������ʻ�����������¹յ�ʱ������һ״̬
         if (annulus_R_memory == 4)
         {
            Exit_loop_R_inflection();
            if(Exit_loop_Flag==0){
                return;
            }
            if(Exit_loop_Flag==1&&Lost_left_Flag==1){
                annulus_R_memory = 5;
            }

         }

         //״̬5 ����ʱ�������¹յ�,���䲹�ߴ���
         if (annulus_R_memory == 5 )
          {
             Exit_loop_R_inflection();
             if(Exit_loop_Flag==1){
                 if(Exit_loop_Y>43){
                     Addingline( 1, 187, Exit_loop_Y-60, Exit_loop_X, Exit_loop_Y);//�˴����߽����������
                     return;
                 }
                 else {
                     Addingline( 1, 187, 50, Exit_loop_X, Exit_loop_Y);//�˴����߽����������
                     return;
                 }
             }
             else if(Lost_right_Flag==1&&Lost_left_Flag==1&&Exit_loop_Flag==0){
                 annulus_R_memory = 6;
             }
             else return;

          }

         //״̬6 ����ʱ���¹յ���ʧ�����ǳ���û��ȫ��������ʱ����Ҫ���ߴ���
         if (annulus_R_memory == 6 )
          {
             gyro_end=jd_sum;
             if(Lost_right_Flag==1&&Lost_left_Flag==1){
                 Addingline( 1,187, Endline+5, 2, 117);
                 return;
             }
             if((Lost_right_Flag==1&&Lost_left_Flag==0)||(gyro_end-gyro_start)>300)//320
             {
                 annulus_R_memory = 7;
             }
             else return;
          }
         //״̬7 ��������
         if (annulus_R_memory == 7)
          {
             if(Upper_right_inflection_Flag==1)
             {
                 Addingline( 2, Upper_right_inflection_X, Upper_right_inflection_Y, 175, 118);
                 return;
             }
             else
             {
                 Addingline( 2, 180, 38, 184, 118); // Ҫ��
             }
             if(Lost_right_Flag==0&&Lost_left_Flag==0){
                 annulus_R_memory =0;
                 annulus_R_Flag=0;
                 return;
             }

          }

     }
}


//===================================================�󳵿�===================================================
//=========������===========
void zebra_crossing(void){
      uint8 i;
//      uint8 num1;
//      uint8 num2;
      uint8 num3;
//      uint8 num4;

      zebra_crossing_flag=0;
//      num1=0;
//      num2=0;
      num3=0;
//      num4=0;
//
//      for(i=3;i<185;i++)
//      {
//          if(imag[Endline+5][i-1]==White&&imag[Endline+5][i]==Black&&imag[Endline+5][i+1]==Black){
//              num4++;
//          }
//
//          if(num1>5){
//              zebra_crossing_flag=1;
//          }
//      }
//
//      for(i=3;i<185;i++)
//      {
//          if(imag[Endline+15][i-1]==White&&imag[Endline+15][i]==Black&&imag[Endline+15][i+1]==Black){
//              num1++;
//          }
//
//          if(num1>5){
//              zebra_crossing_flag=1;
//          }
//      }
//
//      for(i=3;i<185;i++)
//      {
//          if(imag[60][i-1]==White&&imag[60][i]==Black&&imag[60][i+1]==Black){
//              num2++;
//          }
//
//          if(num2>5){
//              zebra_crossing_flag=1;
//          }
//      }

      for(i=3;i<185;i++)
      {
          if(imag[100][i-1]==White&&imag[100][i]==Black&&imag[100][i+1]==Black){
              num3++;
          }

          if(num3>5){
              zebra_crossing_flag=1;
          }
      }

}

//����ʶ��
void Garage(void){
    zebra_crossing();
    Left_garage_flag=0;
    if(zebra_crossing_flag==1)
    {
        annulus_L_Flag=0;
        annulus_L_memory=0;
        Left_garage_flag=1;
    }
    switch(Left_garage_memory){
        case 0:  //����
            if(Lower_left_inflection_Flag==0&&Lower_right_inflection_Flag==0){
                Addingline( 2, 3, Endline+3, 184, 117);   //���ڴ˴��ɵ��ڶ���ս�
            }
            if(Lost_left_Flag==0&&Lost_right_Flag==0&&Lower_left_inflection_Flag==0&&Lower_right_inflection_Flag==0&&Endline<5){
                Left_garage_memory=1;
            }
            break;
        case 1:  //�����������ʻ
            if(Left_garage_flag==1){
                Left_garage_memory=4;
            }
            break;
        case 2:  //��һ�ο������⣬���߹���
//            speed1=0.5;
//            speed2=0.5;
            if(Upper_left_inflection_Flag==1){
                Addingline2(1,Upper_left_inflection_X,Upper_left_inflection_Y);
            }
            else if(Lower_left_inflection_Flag==1){
                Addingline1(1,Lower_left_inflection_X,Lower_left_inflection_Y);
            }
            else{
                Addingline( 1, 29,67, 55, 25);  //����ʵ�ʵ���
            }
            if(Lost_left_Flag==0&&Left_garage_flag==0&&Upper_left_inflection_Flag==0&&zebra_crossing_flag==0&&Lower_left_inflection_Flag==0){
                Left_garage_memory=3;
            }

            break;
        case 3:  //������ʻ
//            speed1=1.5;
//            speed2=1.5;
            if(Left_garage_flag==1){
                Left_garage_memory=4;

            }
            break;
        case 4: //�ٴο�������ʱ�������
//            speed1=-1.7;
//            speed2=-1.7;
            Addingline( 2, Upper_left_inflection_X,Upper_left_inflection_Y,(uint8)right[119] , 119);
            if((Lost_left_Flag==0&&Lost_right_Flag==0)||Endline>50){
                Left_garage_memory =5;
            }
            break;
        case 5: //ɲ��
//            speed1=-2.2;
//            speed2=-2;
//            if(Encoder_speed1<-5){
                Left_garage_memory=7;
//            }
            break;
        case 6://ɲ��
//            speed1=-0.4;
//            speed2=-0.2;
//            if(Encoder_speed1<0){
                Left_garage_memory=7;
//            }
            break;
        case 7:
//            speed1=-0.01;
//            speed2=0;
            break;
    }

}
//---------------------------------------�������---------------------------------
void advanced_regression(int type, int startline1, int endline1, int startline2, int endline2)
 {
    int i = 0;
    int sumlines1 = endline1 - startline1;
    int sumlines2 = endline2 - startline2;
    int sumX = 0;
    int sumY = 0;
    float averageX = 0;
    float averageY = 0;
    float sumUp = 0;
    float sumDown = 0;
    if (type == 0)  //�������
    {
        /**����sumX sumY**/
        for (i = startline1; i <=endline1; i++)
        {
            sumX += i;
            sumY += middle[i];
        }
        for (i = startline2; i <=endline2; i++)
        {
            sumX += i;
            sumY += middle[i];
        }
        averageX = (float)(sumX / (sumlines1 + sumlines2));     //x��ƽ��ֵ
        averageY = (float)(sumY / (sumlines1 + sumlines2));     //y��ƽ��ֵ
        for (i = startline1; i <= endline1; i++)
        {
            sumUp += (middle[i] - averageY) * (i - averageX);
            sumDown += (i - averageX) * (i - averageX);
        }
        for (i = startline2; i <= endline2; i++)
        {
            sumUp += (middle[i] - averageY) * (i - averageX);
            sumDown += (i - averageX) * (i - averageX);
        }
        if (sumDown == 0) parameterB = 0;
        else parameterB = sumUp / sumDown;
        parameterA = averageY - parameterB * averageX;

    }
    else if (type == 1)     //�������
    {
        /**����sumX sumY**/
        for (i = startline1; i <= endline1; i++)
        {
            sumX += i;
            sumY += left[i];
        }
        for (i = startline2; i <= endline2; i++)
        {
            sumX += i;
            sumY += left[i];
        }
        averageX = (float)(sumX / (sumlines1 + sumlines2));     //x��ƽ��ֵ
        averageY = (float)(sumY / (sumlines1 + sumlines2));     //y��ƽ��ֵ
        for (i = startline1; i <= endline1; i++)
        {
            sumUp += (left[i] - averageY) * (i - averageX);
            sumDown += (i - averageX) * (i - averageX);
        }
        for (i = startline2; i <= endline2; i++)
        {
            sumUp += (left[i] - averageY) * (i - averageX);
            sumDown += (i - averageX) * (i - averageX);
        }
        if (sumDown == 0) parameterB = 0;
        else parameterB = sumUp / sumDown;
        parameterA = averageY - parameterB * averageX;
    }
    else if (type == 2)         //�������
    {
        /**����sumX sumY**/
        for (i = startline1; i <= endline1; i++)
        {
            sumX += i;
            sumY += right[i];
        }
        for (i = startline2; i <= endline2; i++)
        {
            sumX += i;
            sumY += right[i];
        }
        averageX = (float)(sumX / (sumlines1 + sumlines2));     //x��ƽ��ֵ
        averageY = (float)(sumY / (sumlines1 + sumlines2));     //y��ƽ��ֵ
        for (i = startline1; i <= endline1; i++)
        {
            sumUp += (right[i] - averageY) * (i - averageX);
            sumDown += (i - averageX) * (i - averageX);
        }
        for (i = startline2; i <= endline2; i++)
        {
            sumUp += (right[i] - averageY) * (i - averageX);
            sumDown += (i - averageX) * (i - averageX);
        }
        if (sumDown == 0) parameterB = 0;
        else parameterB = sumUp / sumDown;
        parameterA = averageY - parameterB * averageX;
    }
}
//------------------------------�ж�ֱ��б���Ƿ���ͬ------------------------------
int Judgment_symbol(float x, float y)
{
    int a;
    a = 0;
    if (x < 0 && y < 0) a = 1;
    if (x >= 0 && y >= 0) a = 1;
    return a;
}
//-----------------------------------------����-----------------------------------
/*                    ===ʹ��������Ȳ���====                                */
void breadth_Addingline(uint8 choice)
{
    uint8 i;
    switch(choice)
    {
        case 1://����
            for(i=119;i>=2;i--)
            {
                if(right[i]-road_width[i]<2)
                    left[i]=2;
                else
                    left[i]=right[i]-road_width[i];
            }
            break;
        case 2://�Ҳ���
            for(i=119;i>=2;i--)
            {
                if(left[i]+road_width[i]>185)
                    right[i]=185;
                else
                    right[i]=left[i]+road_width[i];
            }
            break;
    }
}
//------------------------------------����״̬���������岹��-----------------------------------
//�������յ�Ĳ��ߺ���
void Addingline( uint8 choice, uint8 startX, uint8 startY, uint8 endX, uint8 endY)
{
    y = 0;

    // ֱ�� x = ky + b
    float k = 0;
    float b = 0;
    switch(choice)
    {
      case 1://����
        {
            k = (float)((float)endX - (float)startX) / (float)((float)endY - (float)startY);
            b = (float)startX - (float)startY * k;

            for(y = startY; y < endY; y++)
            {
                if( (uint8)(k * y + b)>185)
                    left[y] = 185;
                else if( (uint8)(k * y + b)<2)
                    left[y] = 2;
                else
                   left[y] = (uint8)(k * y + b);
            }
            break;
        }

      case 2://�Ҳ���
        {
            k = (float)((float)endX - (float)startX) / (float)((float)endY - (float)startY);
            b = (float)startX - (float)startY * k;

            for(y = startY; y < endY; y++)
            {
                if( (uint8)(k * y + b)>185)
                    right[y]=185;
                else if ( (uint8)(k * y + b)<2)
                    right[y]=2;
                else
                right[y]= (uint8)(k * y + b);

            }
            break;
        }

    }
}

//-----------------------------------------����״̬һ����-----------------------------------
void Addingline1( uint8 choice, uint8 startX, uint8 startY)    //�����յ���б�������ӳ�
{

    // ֱ�� x = ky + b
    float k = 0;
    float b = 0;
    int temp=0;
    switch(choice)
    {
      case 1://����
        {

            k = (float)(((float)left[Lower_left_inflection_Y+1] - (float)left[Lower_left_inflection_Y+5]) /(-4));
            b = (float)((float)left[Lower_left_inflection_Y+5]- (float)(Lower_left_inflection_Y+5) * k);

            for(y = startY; y >(Endline+20); y--)
            {

             temp = (int)(k* y + b);
             if(temp<180&&temp>10)
             {
                 left[y]=temp;
             }
            }
            break;
        }

      case 2://�Ҳ���  ������
      {

           k = (float)(((float)right[Lower_right_inflection_Y+1] - (float)right[Lower_right_inflection_Y+5]) /(-4));
           b = (float)((float)right[Lower_right_inflection_Y+5]- (float)(Lower_right_inflection_Y+5) * k);

           for(y = startY; y >(Endline+20); y--)
           {

            temp = (int)(k* y + b);
            if(temp<180&&temp>10){
                right[y]=temp;
            }
           }
           break;
       }

    }
}
//-----------------------------------------���ⲹ��-----------------------------------
void Addingline2( uint8 choice, uint8 startX, uint8 startY)   //�ҵ��Ϲյ���б����������
{

    // ֱ�� x = k*y + b
    float k = 0;
    float b = 0;
    int temp=0;
    switch(choice)
    {
      case 1://����
        {

            k = (float)(((float)left[Upper_left_inflection_Y-2] - (float)left[Upper_left_inflection_Y-5]) /3);
            b = (float)((float)left[Upper_left_inflection_Y-5]- (float)(Upper_left_inflection_Y-5) * k);

            for(y = startY; y<(image_h-3); y++)
            {

             temp = (int)(k* y + b);
             if(temp<185&&temp>2){
                 left[y]=temp;
             }

            }
            break;
        }

//     case 2://�Ҳ���  ������
//        {
//
//            k = (float)(((float)right[Upper_right_inflection_Y-2] - (float)right[Upper_right_inflection_Y-5]) /3);
//            b = (float)((float)right[Upper_right_inflection_Y-5]- (float)(Upper_right_inflection_Y-5) * k);
//            for(y = startY; y<(image_h-3); y++)
//            {
//
//             temp = (int)(k* y + b);
//             if(temp<185&&temp>2){
//                 right[y]=temp;
//             }
//
//            }
//            break;
//        }

    }
}


//float k1,k2,k3;
//===================================================��  ʾ===================================================
void IPS_show(void){
   int i;
  //********************ͼ����ʾ*****************************
ips200_displayimage03x(imag[0], MT9V03X_W, MT9V03X_H);


  //********************������ʾ*****************************
        //��������ѭ�����������߽��
        for (i = 0; i < data_stastics_l; i++)
        {
            ips200_draw_point(points_l[i][0]+2, points_l[i][1],  RGB565_GREEN);
        }
        for (i = 0; i < data_stastics_r; i++)
        {
            ips200_draw_point(points_r[i][0]-2, points_r[i][1],  RGB565_GREEN);
        }

        for (i = Endline; i < image_h-1; i++)
        {
          //  middle[i] = (left[i] + right[i]) >> 1;//������

            //�������������󣬲����ǲ��߻�����״̬����ȫ�����ʹ��һ����ߣ����������������ܸ����������
            //��ȻҲ�ж�����ߵ��ҷ������Ǹ��˸о��ܷ�����������
            ips200_draw_point((uint16)middle[i], (uint16)i,  RGB565_GREEN);
            ips200_draw_point((uint16)left[i], (uint16)i, RGB565_RED);
            ips200_draw_point((uint16)right[i],(uint16) i, RGB565_BLUE);

        }
          if(Endline<115){
              for(int i=119;i>Endline;i--)
               {
                   //ips200_draw_line(middle[scanning_line], 1, middle[scanning_line], 119, RGB565_RED); //����
                   //ips200_draw_line(left[scanning_line], 1, left[scanning_line], 119, RGB565_CYAN); //�������
                   //ips200_draw_line(right[scanning_line], 1, right[scanning_line], 119, RGB565_YELLOW);  //�ҿ�����
                   if(Endline>1&&Endline<120)
                     {
                       ips200_draw_line(1, (uint16)Endline, 187, (uint16)Endline,RGB565_GREEN); //��ֹ��
                     }
                }
          }



  //********************�յ���ʾ*****************************
//  //---------------���¹յ�--------------------
//  if(Lower_left_inflection_Flag==1){
//      ips200_show_string(0,125,"low left");
//      ips200_show_int(0, 140, Lower_left_inflection_X,3);
//      ips200_show_int(0, 155, Lower_left_inflection_Y,3);
//      ips200_draw_point(Lower_left_inflection_X, Lower_left_inflection_Y,  RGB565_BLUE);
//  }
//  //---------------���¹յ�-------------------
//  if(Lower_right_inflection_Flag==1){
//      ips200_show_string(0,170,"low right");
//      ips200_show_int(0, 185, Lower_right_inflection_X,3);
//      ips200_show_int(0, 200, Lower_right_inflection_Y,3);
//      ips200_draw_point(Lower_right_inflection_X, Lower_right_inflection_Y,  RGB565_BLUE);
//  }
//  //---------------���Ϲյ�-------------------
//  if(Upper_left_inflection_Flag==1){
//      ips200_show_string(100,125,"upper left");
//      ips200_show_int(100,140, Upper_left_inflection_X,3);
//      ips200_show_int(100,155, Upper_left_inflection_Y,3);
//      ips200_draw_point(Upper_left_inflection_X, Upper_left_inflection_Y,  RGB565_RED);
//  }
//  //---------------���Ϲյ�--------------------
//  if(Upper_right_inflection_Flag==1){
//      ips200_show_string(100,170,"upper right1");
//      ips200_show_int(100, 185,Upper_right_inflection_X,3);
//      ips200_show_int(100, 200, Upper_right_inflection_Y,3);
//      ips200_draw_point(Upper_right_inflection_X, Upper_right_inflection_Y,  RGB565_RED);
//  }
  //---------------����Ļ�ֽ���--------------------
//  ips200_draw_line(0, 20, 187, 20, RGB565_BLUE);
//  ips200_draw_line(0, 40, 187, 40, RGB565_BLUE);
//  ips200_draw_line(0, 60, 187, 60, RGB565_BLUE);
//  ips200_draw_line(0, 80, 187, 80, RGB565_BLUE);
//  ips200_draw_line(0, 100, 187, 100, RGB565_BLUE);

  //********************����*****************************
//  if(roundabout_Flag==1){
//      ips200_show_string(130,215,"roundabout");
//      ips200_show_int(130, 230,roundabout_X,3);
//      ips200_show_int(130, 245, roundabout_Y,3);
//      ips200_draw_point(roundabout_X, roundabout_Y,  RGB565_RED);
//  }
  //********************��������*****************************
//  if(Exit_loop_Flag==1){
//      ips200_show_string(130,255,"exit");
//      ips200_show_int(130, 270,Exit_loop_X,3);
//      ips200_show_int(130, 285, Exit_loop_Y,3);
//      ips200_draw_point(Exit_loop_X, Exit_loop_Y,  RGB565_RED);
//  }
///////////////////////////////////////////////////////////////////�����ǵ���ʱ�ɿ�������////////////////////////////////////////////////////////////////////////
  //********************��ֱ��*****************************
  if( Right_straight_flag==1){
      ips200_show_string (0, 120, "right straight1");
  }
  else{
      ips200_show_string (0, 120, "right straight0");
  }

  //********************��ֱ��*****************************
  if( Left_straight_flag==1){
      ips200_show_string (0, 140, "Left straight1");
  }
  else{
      ips200_show_string (0, 140, "Left straight0");
  }

  //********************Բ��*****************************
  ips200_show_string (0,160, "annulus_L=");
  ips200_show_int (80,160, annulus_L_memory,2);
  ips200_show_string (100,160, "annulus_R=");
  ips200_show_int (180,160, annulus_R_memory,2);

  //********************s��****************************
  ips200_show_string (0,180, "S_road=");
  ips200_show_int (60,180, S_road_Flag,3);

  //********************ʮ��*****************************
  ips200_show_string (0,200, "Crossroad=");
  ips200_show_int (80,200, Crossroad_memory,3);


  //********************������*****************************
  ips200_show_string (0,220, "zebra=");
  ips200_show_int (50,220, zebra_crossing_flag,3);

  //********************ͣ����־*****************************
  ips200_show_string (100,220, "ting=");
  ips200_show_int (150,220,   tingche_flag,3);

  //********************������*****************************
  ips200_show_string (0, 240, "jd=");
  ips200_show_float(50, 240, jd_sum,4,4);
  ips200_show_string (0,260, "g_start=");
  ips200_show_int (80,260,   gyro_start,3);
  ips200_show_string (100,260, "g_end=");
  ips200_show_int (150,260,   gyro_end,3);

  //********************��ֱ����ʾ*****************************
  if( bend_straight_flag==1){
        ips200_show_string (0, 280, "stra");
    }
    else{
        ips200_show_string (0, 280, "bend");
    }

  //********************ת�������ʾ*****************************
  ips200_show_string (0, 300, "zx_err=");
  ips200_show_float(80, 300, servo.pid_err,2,2);

  //********************������ʾ*****************************
//ips200_show_int(100, 290,Endline,3);
//  ips200_show_float(100, 275,Lost_point_R_scan_line,3,3);
// ips200_draw_point(Upper_left_inflection_X, Upper_left_inflection_Y,  RGB565_BLUE);
// ips200_draw_point(Upper_right_inflection_X, Upper_right_inflection_Y,  RGB565_BLUE);
 //ips200_show_int (15,275, Lost_right_Flag,3);
}

    void  ben_straight(void)
    {
           float k1,k2;
           float l_slope2=0,l_slope3=0,l_distance2=0,l_distance3=0;
           bend_straight_flag=0;
           caculate_distance(100,119,middle,&l_slope3,&l_distance3);
           k2=l_slope3;

           caculate_distance(45,60,middle,&l_slope2,&l_distance2);
           k1=l_slope2;

           if((absolute(k1-k2)<0.15))//&&(Lost_left_Flag==0)&&(Lost_right_Flag==0)
           bend_straight_flag=1;
           else
          bend_straight_flag=0;


}

//===================================================s��===================================================
    void S_road(void)
    {
        float l_slope=0,l_distance=0;  //������б�ʺͽؾ�
        float r_slope=0,r_distance=0;  //������б�ʺͽؾ�

        if(annulus_L_Flag==0&&annulus_R_Flag==0&&S_right_lost_Flag==0&&S_left_lost_Flag==0&&S_road_Flag==0&&S_road_memory==0&&S_right_inflection_Flag==1&&S_left_inflection_Flag==1)
        {
            S_road_Flag=1;
            S_road_memory=1;
        }
        if(S_road_Flag==1)
        {
            if(S_road_memory==1)
            {
                    caculate_distance(Endline+10,image_h-3,left,&l_slope,&l_distance);
                    caculate_distance(Endline+10,image_h-3,right,&r_slope,&r_distance);
                    for(y=image_h-3;y>Endline+10;y--)
                    {

                        if((l_slope*(y)+l_distance)>185)
                            left[y]=185;
                        else if((l_slope*(y)+l_distance)<2)
                            left[y]=2;
                        else
                            left[y]=l_slope*(y)+l_distance;

                        if((r_slope*(y)+r_distance)>185)
                            right[y]=185;
                        else if((r_slope*(y)+r_distance)<2)
                            right[y]=2;
                        else
                            right[y]=r_slope*(y)+r_distance;
                    }

                /*------�ж��Ƿ����S��-------*/
                if(S_right_inflection_Flag==0&&S_left_inflection_Flag==0)
                {
                    S_road_Flag=0;
                    S_road_memory=0;
                }
            }
        }
    }



//===================================================Ԫ��ʶ��===================================================
void Element_recognition(void)
{

    inflection_point();
    ten_inflection();
    left_straight();
    right_straight();
    zebra_crossing();
//    Garage();
    crossroad();
    S_road();
    annulus_L();
    annulus_R();
    middle_line();
    ben_straight();
}


void pian_cha(void)     //ƫ���
{
    int you_pian=0,zuo_pian=0;
    int i;
    for(i=55;i>50;i--)
    {
        if(biao_column>middle[i])//����
           zuo_pian=zuo_pian+(biao_column-middle[i]);
        if(biao_column<middle[i])//�Ҹ�
           you_pian=you_pian+(biao_column-middle[i]);

        if(i==51)
        {
            piancha=(zuo_pian+you_pian)/5;
            }
    }


}




//===================================================ͼ����===================================================

void image_process(void)
{


   if(mt9v03x_finish_flag)           //�ж�һ��ͼ���Ƿ�������
        {
            system_start ();
           // gpio_set_level(P14_0, 0);
            Get_image(mt9v03x_image);    //ͼ��ת��
            binaryzation();              //��ֵ��
//            image_filter(imag);        //�˲�
            image_draw_rectan(imag);     //���ڿ�

            /*===����===*/
            data_stastics_l = 0;
            data_stastics_r = 0;
            xunxian =get_start_point(image_h - 2);
            if (xunxian) //�ҵ�����ˣ���ִ�а�����û�ҵ���һֱ��
            {
                //printf("���ڿ�ʼ������\n");
               search_l_r((uint16)USE_num, imag, &data_stastics_l, &data_stastics_r, start_point_l[0], start_point_l[1], start_point_r[0], start_point_r[1], &Endline);
                //printf("�������ѽ���\n");
                growth_direction();           //����������ȡ
                get_left(data_stastics_l);    //��߽���ȡ
                get_right(data_stastics_r);   //�ұ߽���ȡ
                lost_left();                  //���·������ж�
                lost_right();                 //���·������ж�
                Element_recognition();        //Ԫ���ж�+��ȡ����
                pian_cha();
                Finish_Flag=1;
            }
            image_process_time=system_getval ();//��ȡ��������ʱ��
            mt9v03x_finish_flag=0;              //���ͼ��ɼ���ɱ�־
        }
}


