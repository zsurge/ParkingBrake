# 博思高停车场闸机代码


-------------------------------------------------------------------
常用git命令行指令

Git 全局设置
git config --global user.name "test"

提交代码到服务器
git clone http://120.79.49.246:8020/root/parkingBrake.git
cd parkingBrake
touch README.md
git add README.md
git commit -m "add README"
git push -u origin master

提交已存在的文件夹到服务器
cd existing_folder
git init
git remote add origin http://120.79.49.246:8020/root/parkingBrake.git
git add .
git commit -m "Initial commit"
git push -u origin master



