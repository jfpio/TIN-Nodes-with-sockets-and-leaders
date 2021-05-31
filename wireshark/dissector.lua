tin_analyzer = Proto("TIN",  "TIN analyzer")

node_id = ProtoField.string("tin.node_id", "nodeID")
node_role = ProtoField.string("tin.node_role", "nodeRole")
node_message = ProtoField.string("tin.node_message", "nodeMessage")

tin_analyzer.fields = { node_id, node_role, node_message }

function tin_analyzer.dissector(buffer, pinfo, tree)
    length = buffer:len()
    if length == 0 then return end

    pinfo.cols.protocol = tin_analyzer.name
    local subtree = tree:add(tin_analyzer, buffer(), "Tin analyzer")

    local message_type = buffer(0, 1):string()

    local role
    local role_code = buffer(4, 1):string()
    if role_code == '0'
    then
        role = 'None'
    elseif role_code == '1'
    then
        role = "Leader"
    elseif role_code == '2'
    then
        role = "Vice leader"
    else
        role = "unknown"
    end

    if message_type == '0'
        then
        subtree:add(node_id, buffer(2, 2))
        subtree:add(node_role, role)
        subtree:add(node_message, "Leader message")
    end

    if message_type == '1'
    then
        subtree:add(node_id, buffer(2, id_length))
        subtree:add(node_message, "Id broadcasting message")
    end

    if message_type == '2'
    then
        subtree:add(node_id, buffer(2, id_length))
        subtree:add(node_message, "Session controller kill message")
    end

    if message_type == '3'
    then
        subtree:add(node_id, buffer(2, id_length))
        subtree:add(node_role, role)
        subtree:add(node_message, "More than one")
    end
end

local udp_port = DissectorTable.get("udp.port")
udp_port:add(6000, tin_analyzer)