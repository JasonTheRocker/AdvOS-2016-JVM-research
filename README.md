# AdvOS-2016-JVM-research

#USAGE
Under each benchmark folder

"mvn install" to build the benchmark

"mvn clean" to clean the build

"java -jar target/benchmarks.jar" to run the benchmark

#NPE
npe-nullcheck is to benchmark a simple null check.

npe-throwcatch is to benchmark a manual NPE throw and catch.

npe-catchonly is to benchmark a generated NPE throw and catch.
