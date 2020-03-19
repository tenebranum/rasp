#!/bin/bash
if (($# == 0)) || (($1 == "1"))
    then
    #1 part
        echo task 2 -------------------------------
        ls

        echo task 3 -------------------------------
        ls -r
        
        echo task 4 -------------------------------
        ls -R
        
        echo task 5 -------------------------------
        ls -a
        
        echo task 6.1 -------------------------------
        ls -s --block-size=KB
        echo task 6.2 -------------------------------
        ls -s --block-size=MB
        echo task 6.3 -------------------------------
        ls -s --block-size=GB

        echo task 7.1 -------------------------------
        ls -sS --block-size=KB
        echo task 7.2 -------------------------------
        ls -sS --block-size=MB
        echo task 7.3 -------------------------------
        ls -sS --block-size=GB

        echo task 8 -------------------------------
        ls -d */

        echo task 9 -------------------------------
        ls -St

        echo task 10 -------------------------------
        ls -u

        echo task 11 -------------------------------
        ls ?[aouiey]*.*

        echo task 12 -------------------------------
        echo dirlist.txt
        ls -l >> ~/dirlist.txt
fi

if (($# == 0)) || (($1 == "2"))
    then
    #2 part
        echo task 13 -------------------------------
        df -hl
        
        echo task 14 -------------------------------
        du -s */
        
        echo task 15 -------------------------------
        du -s ~/work/venv/*/
        
        echo task 16 -------------------------------
        du -sh */
        
        echo task 17 -------------------------------
        du -sh ~/work/rasp
        
        echo task 18 -------------------------------
        du -sch ~/work/rasp ~/"Рабочий стол"
fi

if (($# == 0)) || (($1 == "3"))
    then
    #3 part
        echo task 19 -------------------------------
        mkdir ~/linux_lab1
        echo dir created
        
        echo task 20 -------------------------------    
        cd ~/linux_lab1
        echo dir changed
        
        echo task 21 -------------------------------    
        cp ~/dirlist.txt ~/linux_lab1
        echo file copied
        
        echo task 22 -------------------------------    
        rm ~/dirlist.txt
        echo file deleted
fi

if (($# == 0)) || (($1 == "4"))
    then
    #4 part
        echo task 23 -------------------------------
        cd ~/
        mkdir manyfiles
        cd manyfiles
        echo dir created
        
        echo task 24 -------------------------------
        touch a{1..100}
        echo files created
        
        echo task 25 -------------------------------
        mkdir manyfiles{10,30}
        echo dirs created
        
        echo task 26 -------------------------------
        mkdir manyfiles1
        mv a{1..10} manyfiles1/
        echo files moved to manyfiles1

        echo task 27 -------------------------------
        mkdir manyfiles2
        mv a{0..100..5} manyfiles2/
        echo files moved to manyfiles2

        echo task 28 -------------------------------
        rm -f a{0..100..2}
        echo files removed

        echo task 29 -------------------------------
        cp -R manyfiles1 manyfiles3
        echo directory copied

        echo task 30 -------------------------------
        rm -r manyfiles1
        echo directory removed

        echo task 31 -------------------------------
        mkdir Toppler
        touch Toppler/high11 Toppler/low11
        echo dir and files created

        echo task 32 -------------------------------
        mkdir ~/tmp
        cp Toppler/high11 Toppler/low11 ~/tmp
        echo files copied

        echo task 33 -------------------------------
        mv ~/tmp/*11 ~/
        echo files moved

        echo task 34 -------------------------------
        cp -R Toppler ~/tmp/
        echo dir copied

        echo task 35 -------------------------------
        cd ~/tmp/
        mv Toppler Roller
        echo dir renamed

        echo task 36 -------------------------------
        mv -v Roller ~/
        echo dir moved
fi

if (($# == 0)) || (($1 == "5"))
    then
    #5 part
        echo task 37 -------------------------------
        (( sum = 3 + 5 ))
        (( division = 6 / 4 ))
        (( result = 10 / (3 + 2) ))
        echo "3 + 5 = $sum"
        echo "6 / 4 = $division"
        echo "10 / (3 + 2) = $result"

        echo task 38 -------------------------------
        (( res = 75 * 15 / 100 ))
        echo "15% from 75 is $res"
        (( res = 350 * 5 / 100 ))
        echo "5% from 350 is $res"

        echo task 39 -------------------------------
        V1=4
        (( V2 = V1 * 2 ))
        echo "V2 = $V2"

        echo task 40-41 ----------------------------
        VAR1="Робота в Shell"
        echo "VAR1 = $VAR1"

        echo task 42 -------------------------------
        VAR2=$VAR1
        VAR2+=" Bash"
        echo variable created

        echo task 43 -------------------------------
        set -o history
        VAR3=$HISTFILESIZE
        echo variable created

        echo task 44-46 ----------------------------
        unset VAR1
        echo "VAR2 = $VAR2"
        echo "VAR3 = $VAR3"
fi

if (($# == 0)) || (($1 == "6"))
    then
    #6 part
        cd ~/linux_lab1
        
        echo task 47 -----------------------------------
        grep -w 'серпень' dirlist.txt
        grep -w 'серпень' dirlist.txt >> grep_month_name.txt
        
        echo task 48 -----------------------------------
        grep -vw 'серпень' dirlist.txt >> grep_other_months.txt
        echo grep_other_months.txt created
        
        echo task 49 -----------------------------------
        mkdir grep
        mv -t grep/ grep_month_name.txt grep_other_months.txt
        echo dir created and files moved
        
        echo task 50 -----------------------------------
        grep -nR 'stud'
fi

if (($# == 0)) || (($1 == "7"))
    then
    #7 part
        echo task 51 -----------------------------------
        echo "locate"
        locate *bash*.*
        echo "find"
        find /tmp -name "*bash*.*"
        
        echo task 52 -----------------------------------
        find /tmp -mtime -1 
        
        echo task 53 -----------------------------------
        find /tmp -maxdepth 1 -type f -links +1 -printf '%p\n'
        
        echo task 54 -----------------------------------
        export PS1+="\h "
        
        echo task 55 -----------------------------------
        rm -R ~/manyfiles
        echo dir removed
fi

if (($# == 1)) && (($1 == "help"))
	then
		echo arg 1 - ls
		echo arg 2 - df, du
		echo arg 3 - mkdir
		echo arg 4 - mkdir, touch
		echo arg 5 - math operations
		echo arg 6 - grep
		echo arg 7 - locate, find
fi