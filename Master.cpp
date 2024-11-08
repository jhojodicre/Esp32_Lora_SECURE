#include <Arduino.h>
#include "Master.h"
#include "Node.h"
    

Master::Master(byte nodes_number)
{
    _Master     =   0xFF;                   // Direccion del Master.
    _Nodes      =   nodes_number;
    _Node_first =   1;
    _Node_last  =   _Nodes;
    Node Node_test(1);
}
void Master::XXXX()
{
}
void Master::Node_Request()
{
    if (_Node_next == _Node_last)
    {
        _Node_next = _Node_first - 1;
    }
    if (_Node_next <= _Node_last)
    {
        ++_Node_next;
        _Node_requested = _Node_next;
    }
}
void Master::Node_Break()
{
    // switch (_Node_requested)
    // {
    // case 1:
    //     Node1.Update(128, 0);
    //     Node1.Estado();
    //     break;
    // case 2:
    //     Node2.Update(128, 0);
    //     Node2.Estado();
    //     break;
    // case 3:
    //     Node3.Update(128, 0);
    //     Node3.Estado();
    //     break;
    // case 4:
    //     Node4.Update(128, 0);
    //     Node4.Estado();
    //     break;
    // case 5:
    //     Node5.Update(128, 0);
    //     Node5.Estado();
    //     break;
    // case 6:
    //     Node6.Update(128, 0);
    //     Node6.Estado();
    //     break;
    // case 7:
    //     Node7.Update(128, 0);
    //     Node7.Estado();
    //     break;
    // case 8:
    //     Node8.Update(128, 0);
    //     Node8.Estado();
    //     break;
    // case 9:
    //     Node9.Update(128, 0);
    //     Node9.Estado();
    //     break;
    // default:
    //     break;
    // }
}