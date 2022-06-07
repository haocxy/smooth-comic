create table if not exists thumb_cache_for_${name} (
    archive_path text not null,
    entry_path text not null,
    img blob not null,
    constraint c_pk primary key (archive_path, entry_path)
);
