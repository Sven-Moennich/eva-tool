#!/bin/bash

VER=$(cat .last_version.number)

git remote add origin https://github.com/Sven-Moennich/eva-tool.git
git branch -M main
git push -u origin main
