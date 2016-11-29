# AdvOS-2016-JVM-research

#USAGE
Under each benchmark folder

`mvn install` to build the benchmark
  - note: you can set `JAVA_HOME` in `~/.mavenrc` to point to the OpenJDK you built from source instead of the JDK installed on your system.
  - eg: `export JAVA_HOME=/path/to/local_OpenJDK_repo/jdk8/build/linux-x86_64-norm
al-server-release/images/j2sdk-image`

`mvn clean` to clean the build

`java -jar target/benchmarks.jar` to run the benchmark
  - note: be sure to run the `java` binary from your OpenJDK-from-source, and also (if neccessary) set `CLASS_PATH` to point to your OpenJDK-from-source

#NPE
npe-nullcheck is to benchmark a simple null check.

npe-throwcatch is to benchmark a manual NPE throw and catch.

npe-catchonly is to benchmark a generated NPE throw and catch.
