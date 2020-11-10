#/bin/bash

PID=0

function kill_process() {
  kill -9 ${PID}
}

trap kill_process SIGINT

printf "\n\n"

if [ -z ${EGE_ROOT+x} ]; then
	echo "EGE: error: EGE_ROOT is not set"
	exit 2
fi

fail=0
for test in ${EGE_ROOT}/build/root/tests/*; do
	cd build/root
	
	${test} &
	PID="$!"
	fg
	
	if [ $? == 0 ]; then
		printf -- "\e[32mPASS:\e[0m \e[1m%s\e[0m\n" "${test}"
	else
		printf -- "\e[31mFAIL:\e[0m \e[1m%s\e[0m\n" "${test}"
		fail=1
	fi
	cd ../..
done

printf "\n\n"

if [ ${fail} -gt 0 ]; then
	exit 1
fi
