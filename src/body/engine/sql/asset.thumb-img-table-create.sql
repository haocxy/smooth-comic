create table if not exists thumb_cache (
    archive_path text not null,
    entry_path text not null,
    img blob not null
);
