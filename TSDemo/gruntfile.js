module.exports = function(grunt) {
	grunt.initConfig({
        clean: {
            build: ['./Build', './lib']
        },
        copy: {
            dist: {
                files: [
                    {expand: true, cwd: 'Scripts/', src: ['**/*.js', '**/*.ts', '**/*.js.map'], dest: 'Build/'}
                ]
            },
            library: {
            	src: '../ODESolver/src/js-solver.js', dest: 'lib/solver/js-solver.js'
            }
        },
        bower: {
            install: {
        
            }
        },
        ts: {
            default : {
                src: ["Scripts/**/*.ts"],
                options: {
                    additionalFlags: "--declaration --module amd",
                },
            }
        },
        tsd: {
            refresh: {
                options: {
                    command: 'reinstall',
                    path: "typings",
                    latest: true,
                    config: 'tsd.json'
                }
            }
        }
	});

	grunt.loadNpmTasks('grunt-bower-task');
	grunt.loadNpmTasks('grunt-contrib-clean');
	grunt.loadNpmTasks('grunt-contrib-copy');
	grunt.loadNpmTasks('grunt-tsd');
	grunt.loadNpmTasks('grunt-ts');

	grunt.registerTask('build', ['copy:library', 'bower', 'ts', 'copy:dist']);
    grunt.registerTask('installDeps', ['copy:library','bower', 'tsd']);
	grunt.registerTask('default', ['clean', 'build']);

};
