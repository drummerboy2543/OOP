pipeline {
    agent any
    stages {
        stage('build') {
            steps {
                sh 'python --version'
            }
        }
    }
	post {
		success {
				sh 'echo We did it!'
		}
	}
}