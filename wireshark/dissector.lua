tin_analyzer = Proto("TIN",  "TIN analyzer")

node_id = ProtoField.char("tin.node_id", "nodeID")
node_role = ProtoField.char("tin.node_role", "nodeRole")
node_message = ProtoField.char("tin.node_message", "nodeMessage")

tin_analyzer.fields = { node_id, node_role, node_message }

function tin_analyzer.dissector(buffer, pinfo, tree)
    length = buffer:len()
    if length == 0 then return end

    pinfo.cols.protocol = tin_analyzer.name

    local subtree = tree:add(tin_analyzer, buffer(), "Tin analyzer")
    subtree:add(node_id, buffer(0, 2))
    subtree:add(node_role, buffer(2, 1))
    subtree:add(node_message, buffer(3, 1))
end

local udp_port = DissectorTable.get("udp.port")
udp_port:add(6000, tin_analyzer)