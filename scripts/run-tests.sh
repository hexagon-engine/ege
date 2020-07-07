#/bin/bash

printf "\n\n"

if [ -z ${EGE_ROOT+x} ]; then
	echo "EGE: error: EGE_ROOT is not set"
	exit 2
fi

fail=0
for test in ${EGE_ROOT}/build/root/tests/*; do
	if ${test}; then
		printf -- "\e[32mPASS:\e[0m \e[1m%s\e[0m\n" "${test}"
	else
		printf -- "\e[31mFAIL:\e[0m \e[1m%s\e[0m\n" "${test}"
		fail=1
	fi
done

printf "\n\n"

if [ ${fail} -gt 0 ]; then
	exit 1
fi
