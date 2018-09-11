pipeline {
    agent any
    stages {
        stage('build') {
            steps {
                sh 'echo Trying to make a autobuild work'
            }
        }
    }
	post {
		success {
				sh 'echo We did it!'
		}
	}
}