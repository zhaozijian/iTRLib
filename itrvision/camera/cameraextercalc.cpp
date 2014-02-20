#include "cameraextercalc.h"
#define resid_MAX_2 64
namespace itr_vision
{
CameraExterCalc::CameraExterCalc()
{
    //ctor
    H.Init(3,3);
    R.Init(3,6);
    t.Init(3,2);
    N.Init(3,2);

}

CameraExterCalc::~CameraExterCalc()
{
    //dtor
}

CameraExterCalc::CameraExterCalc(const CameraExterCalc &other)
{
    //copy ctor
    this->H=other.H;
    this->V=other.V;
    this->R=other.R;
    this->t=other.t;
    this->N=other.N;
}

BOOL CameraExterCalc::CalcH(VectorFeaturePoint *PointList1,S32 List1Num,VectorFeaturePoint *PointList2,S32 List2Num)
{
    Calculate CalculateObj;
    Numerical NumericalObj;
    //筛选最优 H 时所用
    Matrix best_H(3,3);
    Matrix M(8,9);  //最小二乘矩阵
    M.Set(0);
    Vector pos_2s(3),pos_1(3);
    F32 red_counter,tmp_red,best_counter=0, best_red=0;
    F32 risde;
    //svd 使用
    Matrix matV(9,9);
    Vector S(9);
    //bucketing
    S32 List1Num_q;
    NumericalObj.Round(List1Num/4,List1Num_q);
    S32 bucket_counter[16]= {0};
    S32 bucket[16][List1Num_q];
    //
    F32 tempvalue_u[List1Num],tempvalue_v[List1Num];
    S32 tempID[List1Num];
    S32 matched_num=0;
    for(S32 i=0; i<List1Num; i++)
    {
        if(PointList1[i].ID!=-1)
        {
            tempID[matched_num]=i;
            tempvalue_u[matched_num]=PointList1[i].X;
            tempvalue_v[matched_num]=PointList1[i].Y;
            matched_num++;
        }
    }
    F32 U,V;
    CalculateObj.Max(tempvalue_u,matched_num,U);
    CalculateObj.Max(tempvalue_v,matched_num,V);

    for(S32 i=0; i<matched_num; i++)
    {
        if(tempvalue_u[i]<=U/4&&tempvalue_v[i]<=V/4)
        {
            bucket_counter[0]++;
            bucket[0][bucket_counter[0]]=i;
            continue;
        }
        else if(tempvalue_u[i]<=U/4&&tempvalue_v[i]<=V/2)
        {
            bucket_counter[1]++;
            bucket[1][bucket_counter[1]]=i;
            continue;
        }
        else if(tempvalue_u[i]<=U/4&&tempvalue_v[i]<=3*V/4)
        {
            bucket_counter[2]++;
            bucket[2][bucket_counter[2]]=i;
            continue;
        }
        else if(tempvalue_u[i]<=U/4)
        {
            bucket_counter[3]++;
            bucket[3][bucket_counter[3]]=i;
            continue;
        }
        else if(tempvalue_u[i]<=U/2&&tempvalue_v[i]<=V/4)
        {
            bucket_counter[4]++;
            bucket[4][bucket_counter[4]]=i;
            continue;
        }
        else if(tempvalue_u[i]<=U/2&&tempvalue_v[i]<=V/2)
        {
            bucket_counter[5]++;
            bucket[5][bucket_counter[5]]=i;
            continue;
        }
        else if(tempvalue_u[i]<=U/2&&tempvalue_v[i]<=3*V/4)
        {
            bucket_counter[6]++;
            bucket[6][bucket_counter[6]]=i;
            continue;
        }
        else if(tempvalue_u[i]<=U/2)
        {
            bucket_counter[7]++;
            bucket[7][bucket_counter[7]]=i;
            continue;
        }
        else if(tempvalue_u[i]<=3*U/4&&tempvalue_v[i]<=V/4)
        {
            bucket_counter[8]++;
            bucket[8][bucket_counter[8]]=i;
            continue;
        }
        else if(tempvalue_u[i]<=3*U/4&&tempvalue_v[i]<=V/2)
        {
            bucket_counter[9]++;
            bucket[9][bucket_counter[9]]=i;
            continue;
        }
        else if(tempvalue_u[i]<=3*U/4&&tempvalue_v[i]<=3*V/4)
        {
            bucket_counter[10]++;
            bucket[10][bucket_counter[10]]=i;
            continue;
        }
        else if(tempvalue_u[i]<=3*U/4)
        {
            bucket_counter[11]++;
            bucket[11][bucket_counter[11]]=i;
            continue;
        }
        else if(tempvalue_v[i]<=V/4)
        {
            bucket_counter[12]++;
            bucket[12][bucket_counter[12]]=i;
            continue;
        }
        else if(tempvalue_v[i]<=V/2)
        {
            bucket_counter[13]++;
            bucket[13][bucket_counter[13]]=i;
            continue;
        }
        else if(tempvalue_v[i]<=3*V/4)
        {
            bucket_counter[14]++;
            bucket[14][bucket_counter[14]]=i;
            continue;
        }
        else
        {
            bucket_counter[15]++;
            bucket[15][bucket_counter[15]]=i;
            continue;
        }
    }
    F32 ratio_bucket[16]= {0};
    ratio_bucket[0]=bucket_counter[0]/matched_num;
    for(S32 i=1; i<16; i++)
    {
        ratio_bucket[i]=ratio_bucket[i-1] + bucket_counter[i]/matched_num;
    }
    //RANSAC,calculate H
    S16 b[4]= {0};
    S32 c[4]= {0};
    F32 p,wght,u1,v1,u2,v2;
    S32 q,tmp_k;
    for(S32 i=0; i<10; i++)
    {
        //pick 4 buckets
        for(S32 j=0; j<4; j++)
        {
            NumericalObj.Rand(p);
            for(S32 k=0; k<16; k++)
            {
                if(k==0)
                {
                    if(p<ratio_bucket[0])
                    {
                        q=1;
                        break;
                    }
                }
                else
                {
                    if(ratio_bucket[k-1]<p&&ratio_bucket[k]>=p)
                    {
                        if(bucket_counter[k]>0)
                            q=k;
                        else
                        {
                            tmp_k =k;
                            while(bucket_counter[tmp_k]==0)
                                tmp_k--;
                            q=tmp_k;
                        }
                        break;
                    }
                }
            }
            if(j>0)
            {
                for(S32 z=0; z<j; z++)
                {
                    if(q==b[z])
                    {
                        NumericalObj.Rand(p);
                        for(S32 k=0; k<16; k++)
                        {
                            if(k==0)
                            {
                                if(p<ratio_bucket[0])
                                {
                                    q=1;
                                    break;
                                }
                            }
                            else
                            {
                                if(ratio_bucket[k-1]<p&&ratio_bucket[k]>=p)
                                {
                                    if(bucket_counter[k]>0)
                                        q=k;
                                    else
                                    {
                                        tmp_k =k;
                                        while(bucket_counter[tmp_k]==0)
                                            tmp_k--;
                                        q=tmp_k;
                                    }
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            b[j]=q;
        }
        //pick 4 feature point
        for(S32 j=0; j<4; j++)
        {
            NumericalObj.Rand(p);
            NumericalObj.Ceil(p*bucket_counter[b[j]],q);
            c[j]=tempID[bucket[b[j]][q]];
        }
        //matrix

        for(S32 j=0; j<4; j++)
        {
            u1 = PointList1[c[j]].X;
            v1 = PointList1[c[j]].Y;
            u2 = PointList2[PointList1[c[j]].ID].X;
            v2 = PointList2[PointList1[c[j]].ID].Y;
            wght = PointList2[PointList1[c[j]].ID].Quality;
            M(2*j,3)=-u1*wght;
            M(2*j,4)=-v1*wght;
            M(2*j,5)=-wght;
            M(2*j,6)=u1*v2*wght;
            M(2*j,7)=v1*v2*wght;
            M(2*j,8)=v2*wght;
            M(2*j+1,0)=-u1*wght;
            M(2*j+1,1)=-v1*wght;
            M(2*j+1,2)=-wght;
            M(2*j+1,6)=u1*u2*wght;
            M(2*j+1,7)=v1*u2*wght;
            M(2*j+1,8)=u2*wght;
        }
        //svd

        M.Svdcmp(S,matV);
        for(S32 j=0; j<3; j++)
            for(S32 k=0; k<3; k++)
                H(j,k)=M(j*3+k,8);
        //參差量
        red_counter=0,tmp_red=0;
        for(S32 j=0; j<matched_num; j++)
        {
            pos_1[0]=tempvalue_u[j];
            pos_1[1]=tempvalue_v[j];
            pos_1[2]=1;
            pos_2s=H*pos_1;
            pos_2s[0]/=pos_2s[2];
            pos_2s[1]/=pos_2s[2];
            pos_2s[0]-=PointList2[PointList1[tempID[j]].ID].X;
            pos_2s[1]-=PointList2[PointList1[tempID[j]].ID].Y;
            CalculateObj.MultiSum(pos_2s.GetData(),pos_2s.GetData(),2,risde);

            if(j==c[0]||j==c[1]||j==c[2]||j==c[4])
                risde=0;
            if(risde<resid_MAX_2)   //与王论文相比 risde 未开方
                red_counter++;
            tmp_red+=risde;
        }
        if(i==0)
        {
            best_H=H;
            best_counter=red_counter;
            best_red=tmp_red;
        }
        else
        {
            if((red_counter>best_counter)||((red_counter==best_red)&&(tmp_red<best_red)))
            {
                best_red = tmp_red;
                best_counter = red_counter;
                best_H = H;
            }
        }
    }//end of RANSAC
    H=best_H;
    H.AllMul(1/H(2,2));
    return true;
}
/**
* \brief 通过给定的相机内参数和深度参数D计算运动参数(R,T,N,V)
* \param CameraInterPara 相机内参数
* \param D 深度(单位:米)
* \note 此计算步骤需已完成单应性矩阵计算(既已成功调用CalcHV())
*/
BOOL CameraExterCalc::CalcMotion(CameraInterCalc &CameraInterPara,F32 D)
{
    Matrix tmpA(3,3);
    Matrix tmpApinv(3,3);
    Matrix Hc(3,3);
    Matrix V(3,3),U(3,3);
    Vector d(3);

    CameraInterPara.MatC2P.CopyTo(0,0,3,3,tmpA.GetData());
    tmpA.pinv(tmpApinv);
    Hc=tmpApinv*H*tmpA;

    Hc.CopyTo(0,0,3,3,U.GetData());
    U.Svdcmp(d, V);

    Vector u1(3),u3(3),v1(3),v3(3);
    F32 q1,q2,q3;
    U.CopyRowTo(0, u1.GetData());
    U.CopyRowTo(2, u3.GetData());
    q1=d[0],q2=d[1],q3=d[2];
    V.CopyRowTo(0, v1.GetData());
    V.CopyRowTo(2, v3.GetData());

    F32 err1=q1-q2;
    F32 err2=q2-q3;
    F32 err_ratio=0.005*q2;
    S16 state;
    if (err1<=err_ratio && err2<=err_ratio)
        state=1;
    else
        if(err1>err_ratio&& err2>err_ratio)
            state=3;
        else
            state=2;

    F32 s,stmp;
    U.Det(s);
    V.Det(stmp);
    s*=stmp;

    Matrix R1(3,3);
    Matrix temp33(3,3),v3mat(3,1),v3t(1,3);
    V.CopyRowTo(2, v3t.GetData());
    V.CopyRowTo(2, v3mat.GetData());
    Vector t1(3);
    switch (state)
    {
        case 1:
        {
            R1.CopyFrom(0,0,3,3,H.GetData());
            R1.AllMul(1/q2);
            t1.Set(0);
            R.CopyFrom(0,0,3,3,R1.GetData());
            t.CopyFrom(0,0,1,3,t1.GetData());
        }break;
        case 2:
        {
            temp33=v3mat*v3t;
            R1.CopyFrom(0,0,3,3,H.GetData());
            R1.AllMul(1/q2);
            temp33.AllMul(q3/q1-s);
            R1=R1-temp33;
            t1=u3;
            t1.Mul(D*(q3/q1-s));
            Vector n(3);
            n=v3;
            Vector Sx(3);
            Matrix Vx(3,3);
            v3mat.Svdcmp(Sx,Vx);
            F32 W_n=Sx[0];
            t1.Mul(W_n);
            t1.Mul(-1);
            R.CopyFrom(0,0,3,3,R1.GetData());
            t.CopyFrom(0,0,1,3,t1.GetData());
            N.CopyFrom(0,0,1,3,n.GetData());
        }break;
        case 3:
        {
            F32 r=sqrt((q1*q1-q2*q2)/(q2*q2-q3*q3));
            F32 a=(q1+s*q3*r*r)/(q2+q2*r*r);
            F32 b=-sqrt(1-a*a);
            temp33(0,0)=a;  temp33(0,1)=0;  temp33(0,2)=b;
            temp33(1,0)=0;  temp33(1,1)=1;  temp33(1,2)=0;
            temp33(2,0)=-s*b;  temp33(0,1)=0;  temp33(0,2)=s*a;
            V=V.Tran();
            R1=U*temp33*V;
            Matrix v1mat(3,1),n1(3,1);
            V.CopyRowTo(0, v1mat.GetData());

            n1=v1mat;
            n1.AllMul(r);
            n1=n1+v3mat;

            Vector temp3(3);
            t1=u1; t1.Mul(-b);
            temp3=u3; temp3.Mul(q3/q2-s*a);
            t1=t1+temp3;
            t1.Mul(D);
            //t1=(u1*(-b)+u3*(q3/q2-s*a))*D;
            Vector Sx(3);
            Matrix Vx(3,3);
            //
            N.CopyFrom(0,0,1,3,n1.GetData());
            n1.Svdcmp(Sx,Vx);
            F32 W_n=Sx[0];
            t1.Mul(-1);
            R.CopyFrom(0,0,3,3,R1.GetData());
            t.CopyFrom(0,0,1,3,t1.GetData());


            r=-r;
            b=-b;
            temp33(0,2)=b;
            temp33(2,0)=-s*b;
            Matrix R2(3,3);
            R2=U*temp33*V;
            Matrix n2(1,3);
            n2=v1mat;
            n2.AllMul(r);
            n2=n2+v3mat;

            Vector t2(3);
            t2=u1; t2.Mul(-b);
            temp3=u3; temp3.Mul(q3/q2-s*a);
            t2=t2+temp3; t2.Mul(D);
            //t2=(u1*(-b)+u3*(q3/q2-s*a))*D;
            //
            N.CopyFrom(0,1,1,3,n2.GetData());
            n2.Svdcmp(Sx,Vx);
            W_n=Sx[0];
            t2.Mul(W_n);
            t2.Mul(-1);
            R.CopyFrom(0,3,3,3,R2.GetData());
            t.CopyFrom(0,1,1,3,t2.GetData());

        }break;
    }
    return true;
}
}
