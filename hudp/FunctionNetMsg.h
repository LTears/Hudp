#ifndef HEADER_COMMON_FUNCTIONNETMSG
#define HEADER_COMMON_FUNCTIONNETMSG

#include "TimerSolt.h"
#include "OrderListSolt.h"
#include "SendWndSolt.h"
#include "NetMsg.h"

namespace hudp {

    // ordered messages at the sender 
    class CSenderOrderlyNetMsg : public NetMsg, 
                                 public CSendWndSolt {
    public:
        // the message show be send now
        virtual void ToSend();

        // get the bag ack.
        virtual void AckDone();
    };

    // ordered and reliable messages at the sender 
    class CSenderRelialeOrderlyNetMsg : public NetMsg,
                                        public CSendWndSolt,
                                        public CTimerSolt {
    public:
        // the message show be send now
        virtual void ToSend();

        // get the bag ack.
        virtual void AckDone();

        // time out call back
        virtual void OnTimer();
    };

    // messages at the receiver 
    class CReceiverNetMsg : public NetMsg,
                            public COrderListSolt {
    public:
        // notify superior acceptance 
        virtual void ToRecv();
    };
}
#endif