package app.tourism.ui.screens.auth

import android.content.Context
import android.content.Intent
import androidx.compose.animation.EnterTransition
import androidx.compose.animation.ExitTransition
import androidx.compose.runtime.Composable
import androidx.compose.ui.platform.LocalContext
import androidx.core.content.ContextCompat
import androidx.navigation.compose.NavHost
import androidx.navigation.compose.composable
import androidx.navigation.compose.rememberNavController
import app.tourism.MainActivity
import app.tourism.ui.screens.auth.sign_in.SignInScreen
import app.tourism.ui.screens.auth.sign_up.SignUpScreen
import app.tourism.ui.screens.auth.welcome.WelcomeScreen
import app.tourism.ui.screens.language.LanguageScreen
import kotlinx.serialization.Serializable

// Routes
@Serializable
object Welcome

@Serializable
object SignIn

@Serializable
object SignUp

@Serializable
object Language

@Composable
fun AuthNavigation() {
    val context = LocalContext.current
    val navController = rememberNavController()

    val navigateUp: () -> Unit = { navController.navigateUp() }

    NavHost(
        navController = navController,
        startDestination = Welcome,
        enterTransition = {
            EnterTransition.None
        },
        exitTransition = {
            ExitTransition.None
        }
    ) {
        composable<Welcome>() {
            WelcomeScreen(
                onLanguageClicked = { navController.navigate(route = Language) },
                onSignInClicked = { navController.navigate(route = SignIn) },
                onSignUpClicked = { navController.navigate(route = SignUp) },
            )
        }
        composable<SignIn> {
            SignInScreen(
                onSignInComplete = {
                    navigateToMainActivity(context)
                },
                onBackClick = navigateUp
            )
        }
        composable<SignUp> {
            SignUpScreen(
                onSignUpComplete = {
                    navigateToMainActivity(context)
                },
                onBackClick = navigateUp
            )
        }
        composable<Language> {
            LanguageScreen(
                onBackClick = navigateUp
            )
        }
    }
}

fun navigateToMainActivity(context: Context) {
    val intent = Intent(context, MainActivity::class.java)
    intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TASK or Intent.FLAG_ACTIVITY_NEW_TASK)
    ContextCompat.startActivity(context, intent, null)
}