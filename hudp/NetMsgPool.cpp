#include "NetMsgPool.h"
#include "FunctionNetMsg.h"
using namespace hudp;

// if bigger than it, will reduce half of queue size
static const uint16_t __reduce_limit_size = 10;

CNetMsgPool::CNetMsgPool() {
    //ExpendFree();
}

CNetMsgPool::~CNetMsgPool() {
    size_t size = _free_net_msg_queue.Size();
    NetMsg* net_msg;
    for (int i = 0; i < size; i++) {
        _free_net_msg_queue.Pop(net_msg);
        delete net_msg;
    }
    size = _free_order_queue.Size();
    CSenderOrderlyNetMsg* order_msg;
    for (int i = 0; i < size; i++) {
        _free_order_queue.Pop(order_msg);
        delete order_msg;
    }
    size = _free_reliale_order_queue.Size();
    CSenderRelialeOrderlyNetMsg* reliale_msg;
    for (int i = 0; i < size; i++) {
        _free_reliale_order_queue.Pop(reliale_msg);
        delete reliale_msg;
    }
    size = _free_revceiver_queue.Size();
    CReceiverNetMsg* recv_msg;
    for (int i = 0; i < size; i++) {
        _free_revceiver_queue.Pop(recv_msg);
        delete recv_msg;
    }
}

//void CNetMsgPool::ExpendFree() {
//    NetMsg* msg;
//    for (int i = 0; i < __netmsg_init_pool_size; i++) {
//        msg = new NetMsg();
//        _free_queue.Push(msg);
//    }
//}

void CNetMsgPool::ReduceFree() {
    size_t size = _free_net_msg_queue.Size();
    if (size > __reduce_limit_size) {
        size = size / 2;
        NetMsg* net_msg;
        for (int i = 0; i < size; i++) {
            _free_net_msg_queue.Pop(net_msg);
            delete net_msg;
        }
    }

    size = _free_order_queue.Size();
    if (size > __reduce_limit_size) {
        size = size / 2;
        CSenderOrderlyNetMsg* order_msg;
        for (int i = 0; i < size; i++) {
            _free_order_queue.Pop(order_msg);
            delete order_msg;
        }
    }

    size = _free_reliale_order_queue.Size();
    if (size > __reduce_limit_size) {
        size = size / 2;
        CSenderRelialeOrderlyNetMsg* reliale_msg;
        for (int i = 0; i < size; i++) {
            _free_reliale_order_queue.Pop(reliale_msg);
            delete reliale_msg;
        }
    }

    size = _free_revceiver_queue.Size();
    if (size > __reduce_limit_size) {
        size = size / 2;
        CReceiverNetMsg* recv_msg;
        for (int i = 0; i < size; i++) {
            _free_revceiver_queue.Pop(recv_msg);
            delete recv_msg;
        }
    }
}

NetMsg* CNetMsgPool::GetAckMsg() {
    NetMsg* net_msg;
    if (!_free_net_msg_queue.Pop(net_msg)) {
        net_msg = new NetMsg();
    }

    if (net_msg) {
        net_msg->_phase = PP_HEAD_HANDLE;
        net_msg->_head._flag |= HPF_HIGH_PRI;
        net_msg->_use = true;
        return net_msg;
    }
    return nullptr;
}

NetMsg* CNetMsgPool::GetSendMsg(uint32_t flag) {
    if (flag & HTF_ORDERLY) {
        CSenderOrderlyNetMsg* order_msg;
        if (_free_order_queue.Pop(order_msg)) {
            order_msg->_use = true;
            return (NetMsg*)order_msg;
        }
        return new CSenderOrderlyNetMsg();

    } else if (flag & HTF_RELIABLE) {
        CSenderRelialeOrderlyNetMsg* reliale_msg;
        if (_free_reliale_order_queue.Pop(reliale_msg)) {
            reliale_msg->_use = true;
            return (NetMsg*)reliale_msg;
        }
        return new CSenderRelialeOrderlyNetMsg();

    } else if (flag & HTF_RELIABLE_ORDERLY) {
        CSenderRelialeOrderlyNetMsg* reliale_msg;
        if (_free_reliale_order_queue.Pop(reliale_msg)) {
            reliale_msg->_use = true;
            return (NetMsg*)reliale_msg;
        }
        return new CSenderRelialeOrderlyNetMsg();

    } else if (flag & HTF_NORMAL) {           
        NetMsg* net_msg;
        if (_free_net_msg_queue.Pop(net_msg)) {
            net_msg->_use = true;
            return net_msg;
        }
        return new NetMsg();
    }
    return nullptr;
}

NetMsg* CNetMsgPool::GetRecvMsg() {
    CReceiverNetMsg* recv_msg;
    if (_free_revceiver_queue.Pop(recv_msg)) {
        recv_msg->_use = true;
        return (NetMsg*)recv_msg;
    }
    return new CReceiverNetMsg();
}

void CNetMsgPool::FreeMsg(NetMsg* msg, bool is_recv) {
    if (is_recv) {
        dynamic_cast<CReceiverNetMsg*>(msg)->Clear();
        _free_revceiver_queue.Push(dynamic_cast<CReceiverNetMsg*>(msg));
    }
    if (msg->_head._flag & HPF_NEED_ACK) {
        dynamic_cast<CSenderRelialeOrderlyNetMsg*>(msg)->Clear();
        _free_reliale_order_queue.Push(dynamic_cast<CSenderRelialeOrderlyNetMsg*>(msg));
        return;

    } else if (msg->_head._flag & HPF_IS_ORDERLY) {
        dynamic_cast<CSenderOrderlyNetMsg*>(msg)->Clear();
        _free_order_queue.Push(dynamic_cast<CSenderOrderlyNetMsg*>(msg));
        return;

    } else if (msg->_head._flag) {
        msg->Clear();
        _free_net_msg_queue.Push(msg);
        return;
    }
}