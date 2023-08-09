#!/bin/bash

dname=$1
title=$2

mkdir ${dname}
echo @title $title > ${dname}/${dname}.txt