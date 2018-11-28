#pragma once
#include"Common.h"
#include<unordered_map>
#include<unordered_set>
class TransMng {
public:
    TransMng();

    // The main simulation loop
    void Simulate();

    // The response of a read operation
    void ReceiveResponse(op_t op, int value);


private:
    //------------- Basic stuffs goes here -----------------------
    timestamp_t _now;
    opid_t      _next_opid;

    //------------- Site Status ----------------------------------
    bool site_status[SITE_COUNT];
    std::unordered_map<itemid_t, std::list<siteid_t>> _item_sites;

    //------------- Active Transaction Table ---------------------
    struct trans_table_item {
        timestamp_t                  start_ts;
        bool                         is_ronly;
        bool                         will_abort;
        std::unordered_set<siteid_t> visited_sites;
        trans_table_item(timestamp_t ts, bool ronly) {
            start_ts   = ts;
            is_ronly   = ronly;
            will_abort = false;
        }
    };
    std::unordered_map<transid_t, trans_table_item> _trans_table;
    
    // Queued Ops and Finished ops - recall that there could be no available sites
    std::list<op_t> _queued_ops;

    //--------------------tester cause events----------------------
    void Fail(siteid_t site_id);

    void Recover(siteid_t site_id);

    void DumpAll();

    void Dump(siteid_t site_id);

    //-----------------transaction execution events----------------
    void Begin(transid_t trans_id, bool is_ronly);

    void Finish(transid_t trans_id);

    void Abort(transid_t trans_id);

    void Read(transid_t trans_id, itemid_t item_id);

    void Write(transid_t trans_id, itemid_t item_id, int value);
};