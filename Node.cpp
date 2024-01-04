#include <Arduino.h>
#include "Node.h"

Node::Node(){
    node +=1;
    
}
void Node::Ack(char functionCode){
    nodeACK ++;
    if(nodeACK == 3) nodeACK = 0;
    switch(functionCode){
        case '0':
            Zone_A_FAL = true;
            break;
        case '1':
            Zone_A_FAL = false;
            break;
        case '2':
            Zone_B_FAL = true;
            break;
        default:
            break;
    }
}
int Node::GetAckNum(){
    return nodeACK;
}
void Node::Coming(char nodeCode){
    switch(nodeCode){
        case '1':
            Zone_A_FAL = true;
            break;
        case '2':
            Zone_B_FAL = true;
            break;
        case '3':
            break;
        default:
            break;
    }

}