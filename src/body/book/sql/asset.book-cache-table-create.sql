create table if not exists book_cache (
    entry_path text not null,
    width int not null,
    height int not null,
    constraint c_pk primary key (entry_path)
);
