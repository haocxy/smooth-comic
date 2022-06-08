create table if not exists thumb_cache (
    entry_path text not null,
    img blob not null,
    constraint c_pk primary key (entry_path)
);
