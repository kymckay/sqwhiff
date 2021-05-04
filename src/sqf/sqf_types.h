#pragma once

// Actual data types that exist in SQF at runtime, used for interpreting/analysis
enum SQFType {
    array,
    boolean,
    code,
    config,
    control,
    diary_record,
    display,
    eden_entity,
    eden_id,
    editor_object,
    group,
    hashmap,
    hasmapkey,
    location,
    nan,
    nspace,
    number,
    object,
    script,
    side,
    string,
    structured_text,
    task,
    team_member
};