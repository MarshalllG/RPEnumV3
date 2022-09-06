#include "RP_Enumeration.hh"

// Insert here the code in the method of class EnumerationWLOpt

void EnumerationRPOpt::First()
{ // insert first router

    point router;
    router.row = 0;
    router.col = 0;

    while (in.IsWall(router.row, router.col))
    {
        if (router.col < in.Columns()-1)
            router.col++;
        else if (router.row < in.Rows()-1 && router.col == in.Columns()-1) 
            router.row++;
    }

    out.InsertRouter(router.row, router.col);
    out.ConnectRouterWithBackbone(router);
}


bool EnumerationRPOpt::Next()
{
    point router;
    unsigned k = 0;

    cout << "Next: " << endl;
    cout << out << endl;

    for (unsigned r = 0; r < in.Rows(); r++)
    {
        for (unsigned c = 0; c < in.Columns(); c++)
        {
            // cout << "r c: " << r << " " << c << endl;
            // cout << "k: " << k << endl;

            if (out.Cells()[r][c] == 'r')
            {
                router.col = c;
                router.row = r;
            
                if (out.Cell(out.NextCell(r,c).row, out.NextCell(r,c).col) == 'r')
                {
                    if (in.IsGridPoint(out.NextCell(r,c).row, out.NextCell(r,c).col))
                    {
                        cout << "NextCell is" << out.NextCell(r,c).row << " " << out.NextCell(r,c).col << endl;
                        k++;
                        cout << "k: " << k << endl;
                    }
                }
                else if (in.IsGridPoint(out.NextCell(r,c).row, out.NextCell(r,c).col))
                {   
                    out.RemoveRouter(router);
                    cout << "Rimuovo router: " << router.row << " " << router.col << endl;

                    out.InsertRouter(out.NextCell(r,c).row, out.NextCell(r,c).col);
                    cout << "Inserisco router: " << out.NextCell(r,c).row << " " << out.NextCell(r,c).col << endl;
                
                    // for (c1 = 0; c1 < k; c1++)
                    // {
                    //     router.row = r;
                    //     router.col = c-k+c1;
                    //     if (router.col < 0)
                    //         router.col = in.Columns()-1;
                    //     out.RemoveRouter(router);
                    //     cout << "Rimuovo router: " << router.row << " " << router.col << endl;

                    //     out.InsertRouter(r, c1);
                    //     cout << "Inserisco router: " << r << " " << c1 << endl;

                    // }

                    int count;
                    point temp;
                    temp.row = r;
                    temp.col = c;

                    for (unsigned m = 0; m < k; m++)
                    {
                        count = m - k;
                        cout << "count: " << count << endl;
                        for (int i = 0; i < abs(count); i++)
                        {
                            out.RemoveRouter(temp = out.PreviousCell(temp.row, temp.col));
                            cout << "Rimuovo router: " << temp.row << " " << temp.col << endl;
                        }
                    }

                    count = 0;
                    for (unsigned r1 = 0; r1 < in.Rows(); r1++)
                    {
                        for (unsigned c1 = 0; c1 < in.Columns(); c1++)
                        {
                            if (count == k)
                                break;
                            out.InsertRouter(r1,c1);
                            cout << "Inserisco router: " << r1 << " " << c1 << endl;

                            count++;
                        }
                    }


                    // for (s1 = 0; s1 < c; s1++)
                    // {
                    //     out.RemoveShift(s-c+s1);
                    //     out.InsertShift(s1);
                    // }


                    return true;
                    
                }
            }
        }
    }

    cout << "out:" << endl << out << endl;

    cout << "out.RoutersSize() is " << out.RoutersSize() << endl;

    for (unsigned i = 0; i < out.RoutersSize(); i++)
        cout << "routers[" << i << "]: " << out.Router(i).row << out.Router(i).col << endl;

    if (out.RoutersSize() == in.Rows()*in.Columns())
    {
        return false;
    }

    unsigned routers = out.RoutersSize();
    
    cout << "RemoveandRestart" << endl;
    // cout << out;
    out.RemoveAllRoutersAndRestartBackbone();
    // cout << "RemoveDone" << endl;
    // cout << out;
    unsigned n = 0;
    for (unsigned r = 0; r < in.Rows(); r++)
    {
        for (unsigned c = 0; c < in.Columns(); c++)
        {
            if (n == routers+1)
                break;
            out.InsertRouter(r,c);
            n++;
        }
    }
    

    // cout << "Inserimento router: " << endl;
    cout << "Start with " << out.RoutersSize() << " routers" << endl;
    return true;
}


bool EnumerationRPOpt::Feasible()
{
    // check budget
    if (out.RemainingBudget() < 0)
        return false;

    // check routers
    for (unsigned i = 0; i < out.RoutersSize(); i++)
        if (in.IsWall(out.Router(i).row, out.Router(i).col))
            return false;
    
    return true;
}


int EnumerationRPOpt::Cost()
{
    return 0;
}













           // else if (k == out.RoutersSize()-1 && k >= 1)
                // {
                //     unsigned routers = out.RoutersSize();
                //     out.RemoveAllRoutersAndRestartBackbone();
                    
                //     unsigned num = 0;
                //     for (unsigned r1 = 0; r1 < in.Rows(); r1++)
                //     {
                //         for (unsigned c1 = 0; c1 < in.Columns(); c1++)
                //         {
                //             if (num == routers-1)
                //                 break;
                //             out.InsertRouter(r1,c1);
                //             num++;
                //         }
                //     }
                //     out.InsertRouter(r,c+1);
                //     cout << "Inserisco router: " << r << " " << c+1 << endl;
                //     return true;
                // }