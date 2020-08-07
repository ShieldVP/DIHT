#!/bin/bash

# Variables
path=${1/#\~/$HOME\/}
extension=$2
backup_dir_name=$3
backup_archive_name=$4
# Solving collisions of given names
if [ -d $backup_dir_name ]; then
    id=$(date +'%Y%m%d%k%M%S')
    backup_dir_name=$backup_dir_name$id
    backup_archive_name=${backup_archive_name%.tar.gz}$id'.tar.gz'
fi
# Copying files from folders not heigher than given path with given extension
mkdir $backup_dir_name
find ${path} -name '*.'${extension} -exec cp --backup=numbered -- {} ${backup_dir_name}/ > /dev/null 2>&1 \;
# Creating resulting archive
tar -zcf ${backup_archive_name} ${backup_dir_name}

echo 'done'
exit 0
