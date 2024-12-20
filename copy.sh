outputDir="MathCat"
rulesDir="c-example/Rules"
headerFile="c-example/mathcat.h"

if [ -n "$1" ]; then 
    outputDir="MathCat/$1"
fi

mkdir -p ${outputDir}

cp  -r "c-example/Rules" "${outputDir}/"

cd target
for dir in *; do
    if [ -d "$dir" ] ; then
        echo $dir
        for file in $(find "$dir" -name "*mathcat_c.a" -o -name "*mathcat_c.so" -o -name "*mathcat_c.dll*");
        do
            mkdir "../${outputDir}/${dir}"
            mkdir "../${outputDir}/${dir}/include/"
            cp  "../${headerFile}" "../${outputDir}/${dir}/include/"
            mkdir "../${outputDir}/${dir}/lib/"
            cp  "$file" "../${outputDir}/${dir}/lib/"
            echo $file
        done
    fi
done
cd -

cd ${outputDir}
for dir in *; do
    if [ -d "$dir" ] ; then
        echo $dir
		if [ "${dir}" = "aarch64-linux-android" ] ; then
			mv "aarch64-linux-android" "mathcat-android-arm64"
		elif [ "$dir" = "armv7-linux-androideabi" ] ; then
			mv "armv7-linux-androideabi" "mathcat-android-arm"
		fi
    fi
done
