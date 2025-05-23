@echo off
cd /d F:\vulkan-2d-sidescroller

REM Initialize git repo if not already done
if not exist ".git" (
    git init
    git remote add origin https://github.com/LukeCoulson1/vulkan-2d-sidescroller
)

git add .
git commit -m "Update project"
git push -u origin main

pause